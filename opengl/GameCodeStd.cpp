#include "GameCodeStd.h"
#include <crtdbg.h>
#include <string>
#include <Windows.h>
#include "MemoryMacros.h"

void* operator new(size_t size, int memType, const char* filename, int lineNum)
{
	// We have to do this old-school since we're not allowed to dynamically allocate memory here.
	char buffer[2048];
	int index = 0;
	index += strlen(ultoa(size, buffer, 10));
	strcpy(buffer + index, " -> ");
	index += 4;
	strcpy(buffer + index, filename);
	index += strlen(filename);
	buffer[index] = ':';
	++index;
	index += strlen(itoa(lineNum, buffer + index, 10));
	buffer[index] = '\n';
	++index;
	buffer[index] = '\0';
	++index;
	OutputDebugStringA(buffer);

	return _malloc_dbg(size, memType, filename, lineNum);
}

void operator delete(void* pMemory)
{
	_free_dbg(pMemory, 1);
}

void* operator new[](size_t size, int memType, const char* filename, int lineNum)
{
	// We have to do this old-school since we're not allowed to dynamically allocate memory here.
	char buffer[2048];
	int index = 0;
	index += strlen(ultoa(size, buffer, 10));
	strcpy(buffer + index, " -> ");
	index += 4;
	strcpy(buffer + index, filename);
	index += strlen(filename);
	buffer[index] = ':';
	++index;
	index += strlen(itoa(lineNum, buffer + index, 10));
	buffer[index] = '\n';
	++index;
	buffer[index] = '\0';
	++index;
	OutputDebugStringA(buffer);

	return _malloc_dbg(size, 1, filename, lineNum);
}

void operator delete[](void* pMemory)
{
	_free_dbg(pMemory, 1);
}
