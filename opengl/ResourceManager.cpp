#include "ResourceManager.h"






ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}

WeakResourcePtr ResourceManager::AttachResource(StrongResourcePtr p)
{
	resourceList.push_front(p);
	return WeakResourcePtr(p);
}


void ResourceManager::UpdateResources(unsigned long deltaMs)
{
	ResourceList::iterator it = resourceList.begin();
	while (it != resourceList.end())
	{
		shared_ptr<Resource> pCurrResource = (*it);
		++it;
		pCurrResource->VOnUpdate(deltaMs);
	}
}

void ResourceManager::ClearAllResources(void)
{
	resourceList.clear();
}
