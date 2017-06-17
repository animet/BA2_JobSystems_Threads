#pragma once
#include "KernelSchedulerThread.h"

#define MAX_KERNEL_SCHEDULER_THREADS 32

struct Job;

class Scheduler
{
public:
	~Scheduler();

	static Scheduler* GetInstance();

	void Initialize();

	bool PushJob(Job* job);

	bool AreAllSchedulerThreadsReadyForProcessing() const;

	/*bool PopJob(Job** OUT_Job);

	size_t JobCount();*/

private:

	Scheduler();

	KernelSchedulerThread* getMinSizedJobQueue();

	static Scheduler* m_instance;

	u_int m_numOfProcessors;
	u_int m_numOfSchedulerThreads;
	KernelSchedulerThread m_schedulerThreads[MAX_KERNEL_SCHEDULER_THREADS];
	//KernelSchedulerThread m_systemCallThreads[16]; //Try first wirth worker kernel threads like
	std::mutex m_JobsMutex;
	//std::queue<Job*> m_Jobs;
	//DEBUG:
	std::atomic_uint m_pushedJobs;
};

