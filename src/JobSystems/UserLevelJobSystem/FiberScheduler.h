#pragma once

#include "FiberSchedulerThread.h"

#define MAX_FIBER_SCHEDULER_THREADS 32

class FiberScheduler
{
public:
	~FiberScheduler();

	static FiberScheduler* GetInstance();

	void Initialize();

	void ActivateSchedulerThreads();

	bool PushJob(Job* job);

	bool AreAllSchedulerThreadsReadyForProcessing() const;

	FiberWorker* GetFiberByPointer(void* fiberPointer);

private:

	FiberScheduler();

	FiberSchedulerThread* getMinSizedJobQueue();

	static FiberScheduler* m_instance;
	static std::mutex m_initInstanceMutex;

	u_int m_numOfProcessors;
	u_int m_numOfSchedulerThreads;
	FiberSchedulerThread m_schedulerThreads[MAX_FIBER_SCHEDULER_THREADS];
};

