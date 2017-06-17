#pragma once

#include "UMSWorkerThread.h"
#include "Job.h"
#include "UMSSchedulerThreadParameter.h"


#define MAX_TASKS_PER_SCHEDULER_THREAD 256

class UMSScheduler;
//class UMSWorkerThread;

class UMSSchedulerThread
{
public:

	UMSSchedulerThread();
	~UMSSchedulerThread();

	//Kernel-Level
	HANDLE m_Thread;
	DWORD_PTR m_ProcessorAffinity;
	DWORD m_ThreadId;
	
	void Initialize(UMSScheduler* scheduler);

	DWORD ThreadProc();

	void SchedulingProc();

	bool PushReadyWorker(UMSWorkerThread* worker);

	bool PopReadWorker(UMSWorkerThread** OUT_worker);

	size_t ReadyWorkerQueueSize();

private:

	static DWORD WINAPI StaticThreadProc(void* p);

	//UMS
	std::mutex m_readyWorkersMutex;
	std::queue<UMSWorkerThread*> m_readyWorkerThreads;
	UMSScheduler* m_scheduler;
	UMS_SCHEDULER_STARTUP_INFO m_startupInfo;
	UMSSchedulerThreadParameter m_procParam;
	//Task* m_tasks[MAX_TASKS_PER_SCHEDULER_THREAD];
};