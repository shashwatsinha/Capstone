#pragma once
#define GCC_MEMORYPOOL_DECLARATION(__defaultNumChunks__) \
    public: \
		static MemoryPool* s_pMemoryPool; \
		static void InitMemoryPool(unsigned int numChunks = __defaultNumChunks__, const char* debugName = 0); \
		static void DestroyMemoryPool(void); \
        static void* operator new(size_t size); \
        static void operator delete(void* pPtr); \
        static void* operator new[](size_t size); \
        static void operator delete[](void* pPtr); \
    private: \


//---------------------------------------------------------------------------------------------------------------------
// This macro defines the definition for the overloaded new & delete operators on a class meant to be pooled with a
// memory pool.  It is meant to work specifically with the MemoryPool class.  To use it, call this macro from the cpp
// file where your class function definitions are.
//	- _className_:		The name of this class.
//---------------------------------------------------------------------------------------------------------------------
#define GCC_MEMORYPOOL_DEFINITION(_className_) \
	MemoryPool* _className_::s_pMemoryPool = NULL;\
	void _className_::InitMemoryPool(unsigned int numChunks, const char* debugName) \
	{ \
		if (s_pMemoryPool != NULL) \
		{ \
			/*GCC_ERROR("s_pMemoryPool is not NULL.  All data will be destroyed.  (Ignorable)");*/ \
			SAFE_DELETE(s_pMemoryPool); \
		} \
		s_pMemoryPool = GCC_NEW MemoryPool; \
		if (debugName) \
			s_pMemoryPool->SetDebugName(debugName); \
		else \
			s_pMemoryPool->SetDebugName(#_className_); \
		s_pMemoryPool->Init(sizeof(_className_), numChunks); \
	} \
	void _className_::DestroyMemoryPool(void) \
	{ \
		/*GCC_ASSERT(s_pMemoryPool != NULL); */\
		SAFE_DELETE(s_pMemoryPool); \
	} \
    void* _className_::operator new(size_t size) \
    { \
        /*GCC_ASSERT(s_pMemoryPool);*/ \
        void* pMem = s_pMemoryPool->Alloc(); \
        return pMem; \
    } \
    void _className_::operator delete(void* pPtr) \
    { \
       /* GCC_ASSERT(s_pMemoryPool); */\
        s_pMemoryPool->Free(pPtr); \
    } \
    void* _className_::operator new[](size_t size) \
    { \
        /*GCC_ASSERT(s_pMemoryPool);*/ \
        void* pMem = s_pMemoryPool->Alloc(); \
        return pMem; \
    } \
    void _className_::operator delete[](void* pPtr) \
    { \
       /* GCC_ASSERT(s_pMemoryPool);*/ \
        s_pMemoryPool->Free(pPtr); \
    } \


//---------------------------------------------------------------------------------------------------------------------
// This macro defines a static class that automatically initializes a memory pool at global startup and destroys it at
// global destruction time.  Using this gets around the requirement of manually initializing and destroying the memory
// pool yourself.
//---------------------------------------------------------------------------------------------------------------------
#define GCC_MEMORYPOOL_AUTOINIT_DEBUGNAME(_className_, _numChunks_, _debugName_) \
class _className_ ## _AutoInitializedMemoryPool \
{ \
public: \
	_className_ ## _AutoInitializedMemoryPool(void); \
	~_className_ ## _AutoInitializedMemoryPool(void); \
}; \
_className_ ## _AutoInitializedMemoryPool::_className_ ## _AutoInitializedMemoryPool(void) \
{ \
	_className_::InitMemoryPool(_numChunks_, _debugName_); \
} \
_className_ ## _AutoInitializedMemoryPool::~_className_ ## _AutoInitializedMemoryPool(void) \
{ \
	_className_::DestroyMemoryPool(); \
} \
static _className_ ## _AutoInitializedMemoryPool s_ ## _className_ ## _AutoInitializedMemoryPool; \

#define GCC_MEMORYPOOL_AUTOINIT(_className_, _numChunks_) GCC_MEMORYPOOL_AUTOINIT_DEBUGNAME(_className_, _numChunks_, #_className_)