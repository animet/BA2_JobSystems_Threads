#pragma once
#include "KernelWorkerThread.h"

#define MAX_WORKERTHREADS_PER_SCHEDULER_THREAD 128

class Scheduler;

class KernelSchedulerThread
{
public:
	KernelSchedulerThread();
	~KernelSchedulerThread();

	//Kernel-Level
	HANDLE m_Thread;
	DWORD_PTR m_ProcessorAffinity;
	DWORD m_ThreadId;

	void Initialize(Scheduler* scheduler);

	DWORD ThreadProc();

	bool IsReadyForProcessing() const;

	KernelWorkerThread* FrontReadyWorker();

	void PushReadyWorker(KernelWorkerThread* worker);

	Job* FrontJob();

	bool PushJob(Job* job);

	bool PopJob(Job** OUT_Job);

	size_t JobCount();

	size_t JobCount_Unsafe() const;

private:

	static DWORD WINAPI StaticThreadProc(void* p);

	bool PopReadyWorker(KernelWorkerThread** OUT_worker); //Fibers do not need this...

	Scheduler* m_scheduler;
	
	KernelWorkerThread m_workers[MAX_WORKERTHREADS_PER_SCHEDULER_THREAD];
	std::mutex m_scheduleMutex;
	std::condition_variable m_scheduleCondition; //Just for signal
	//bool m_readyForScheduling;
	std::mutex m_readyWorkersMutex; //Need for mutex (no need when using fibers)
	std::queue<KernelWorkerThread*> m_readyWorkers;
	std::mutex m_JobsMutex;
	//std::condition_variable m_JobsCondition; -> would need own mutex "doWorkMutex"
	std::queue<Job*> m_Jobs;

	//DEBUG:
	static std::atomic_uint m_pushedJobs;
	//static std::atomic_uint m_pushedJobs
	static std::atomic_uint m_popedJobs;
	bool m_isReadyForProcessing;
};

