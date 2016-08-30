#pragma once
#include "Process.h"
class ProcessManager
{
	typedef std::list<StrongProcessPtr> ProcessList;
	ProcessList processList;
public:
	ProcessManager();
	~ProcessManager();

	void UpdateProcesses(unsigned long deltaMs);  // updates all attached processes
	WeakProcessPtr AttachProcess(StrongProcessPtr pProcess);  // attaches a process to the process mgr
	void ClearAllProcesses(void);
	
};

