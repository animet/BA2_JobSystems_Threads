#pragma once

struct Job;
class KernelThreadScheduler;

class KernelThreadWorker
{
public:
	KernelThreadWorker();
	~KernelThreadWorker();

	//Kernel-Level
	HANDLE m_Thread;
	DWORD_PTR m_ProcessorAffinity;
	DWORD m_ThreadId;

	void Initialize(KernelThreadScheduler* scheduler);

	DWORD ThreadProc();

	bool IsReadyForProcessing() const;

	void Activate();

	Job* FrontJob();

	bool PushJob(Job* job);

	bool PopJob(Job** OUT_Job);

	size_t JobCount();

	size_t JobCount_Unsafe() const;

private:

	static DWORD WINAPI StaticThreadProc(void* p);

	KernelThreadScheduler* m_scheduler;

	std::mutex m_scheduleMutex;
	std::condition_variable m_scheduleCondition;

	std::mutex m_JobsMutex;
	std::queue<Job*> m_Jobs;

	bool m_isReadyForProcessing;

	//DEBUG:
	//static std::atomic_uint m_pushedJobs;
	//static std::atomic_uint m_popedJobs;
	
};

