#pragma once

//Inspiration: https://msdn.microsoft.com/en-us/library/windows/desktop/ms686919(v=vs.85).aspx

struct Job;
class FiberScheduler;
class FiberSchedulerThread;
class AssistantKernelWorkerThread;

class FiberWorker
{
public:
	FiberWorker();
	~FiberWorker();

	//Fiber:
	void* m_FiberPointer;
	//AssistantKernelWorkerThread* m_AssignedAssistantThread;

	void Initialize(FiberSchedulerThread* schedulerThread);

	void SetJob(Job* job);

	FiberSchedulerThread* GetFiberSchedulerThread();

private:

	bool m_hasWork;
	Job* m_CurrentJob;
	FiberSchedulerThread* m_SchedulerThread;

	void Proc(void* p);

	static void WINAPI StaticProc(void* p);
};

