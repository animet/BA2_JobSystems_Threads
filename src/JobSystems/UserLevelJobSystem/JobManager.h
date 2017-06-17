#pragma once

#include "Job.h"

#define MAX_JOBS 100000

class JobManager
{
public:
	~JobManager();

	static JobManager* GetInstance();

	Job* CreateJob();

	Job* CreateChildJob(Job* parent);

	void FinishJob(Job* job);

	void WaitForJob(Job* job, int targetWorkCounter);

	unsigned int GetFinishedJobs() const;

private:

	JobManager();

	void DecrementParentWorkCounter(Job* parent);

	static std::mutex m_instanceMutex;
	static JobManager* m_instance;

	//https://blog.molecular-matters.com/2015/09/08/job-system-2-0-lock-free-work-stealing-part-2-a-specialized-allocator/
	Job m_Jobs[MAX_JOBS];
	std::mutex m_JobsMutex; //DO NOT NEED THIS (just for testing)
	std::atomic_uint m_currentJobCount;
	std::atomic_uint m_finishedJobs;
};

