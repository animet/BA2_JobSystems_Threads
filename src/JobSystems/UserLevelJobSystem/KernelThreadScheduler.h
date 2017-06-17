#pragma once
#include "KernelThreadWorker.h"

#define MAX_KERNELTHREAD_WORKERS 32

struct Job;

class KernelThreadScheduler
{
public:
	~KernelThreadScheduler();

	static KernelThreadScheduler* GetInstance();

	void Initialize();

	void ActivateWorkers();

	bool PushJob(Job* job);

	bool AreAllWorkerThreadsReadyForProcessing() const;

private:

	KernelThreadScheduler();

	KernelThreadWorker* getMinSizedJobQueue();

	static KernelThreadScheduler* m_instance;
	static std::mutex m_initInstanceMutex;

	u_int m_numOfProcessors;
	u_int m_numOfWorkerThreads;
	KernelThreadWorker m_workerThreads[MAX_KERNELTHREAD_WORKERS];
	std::mutex m_JobsMutex;
	//DEBUG:
	//std::atomic_uint m_pushedJobs;

};

