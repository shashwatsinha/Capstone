#include "ProcessManager.h"



ProcessManager::ProcessManager()
{
}


ProcessManager::~ProcessManager()
{
}

WeakProcessPtr ProcessManager::AttachProcess(StrongProcessPtr p)
{
	processList.push_front(p);
	return WeakProcessPtr(p);
}

void ProcessManager::UpdateProcesses(unsigned long deltaMs)
{
	ProcessList::iterator it = processList.begin();
	while (it != processList.end())
	{
		shared_ptr<Process> pCurrProcess = (*it);
		++it;
		pCurrProcess->VOnUpdate(deltaMs);
	}
}

void ProcessManager::ClearAllProcesses(void)
{
	processList.clear();
}
