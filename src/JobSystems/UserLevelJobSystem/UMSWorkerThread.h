#pragma once

struct Job;
class UMSScheduler;
struct UMSWorkerThreadParameter;

class UMSWorkerThread
{
public:
	UMSWorkerThread();
	~UMSWorkerThread();

	DWORD m_ThreadId;
	HANDLE m_Thread;
	PVOID m_Context;

	Job* m_CurrentTask;

	void Initialize(UMSScheduler* scheduler);

	DWORD Proc(UMSWorkerThreadParameter* param);

	static DWORD WINAPI StaticProc(void* p);

	void Work();

private:

	UMSScheduler* m_scheduler;
	
};

