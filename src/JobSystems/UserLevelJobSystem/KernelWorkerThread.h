#pragma once

//Inspiration: https://msdn.microsoft.com/en-us/library/windows/desktop/ms686919(v=vs.85).aspx

struct Job;
class KernelSchedulerThread;

class KernelWorkerThread
{
public:
	KernelWorkerThread();
	~KernelWorkerThread();

	//Kernel-Level
	HANDLE m_Thread;
	DWORD_PTR m_ProcessorAffinity;
	DWORD m_ThreadId;


	void Initialize(KernelSchedulerThread* schedulerThread);

	void SetJob(Job* Job);

private:

	std::mutex m_workMutex;
	std::condition_variable m_workCondition;
	std::atomic_int m_JobReady;
	Job* m_CurrentJob;
	KernelSchedulerThread* m_SchedulerThread;

	DWORD ThreadProc();

	static DWORD WINAPI StaticThreadProc(void* p);
};

