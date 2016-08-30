#pragma once
#include "Resource.h"
class ResourceManager
{
	typedef std::list<StrongResourcePtr> ResourceList;
	ResourceList resourceList;
public:
	ResourceManager();
	~ResourceManager();

	void UpdateResources(unsigned long deltaMs);  // updates all attached Resourcees
	WeakResourcePtr AttachResource(StrongResourcePtr pResource);  // attaches a Resource to the Resource mgr
	void ClearAllResources(void);

};
