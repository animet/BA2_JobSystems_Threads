#include "stdafx.h"
#include "JobManager.h"


JobManager::JobManager()
	: m_currentJobCount(0), m_finishedJobs(0)
{
}

JobManager::~JobManager()
{
}

JobManager * JobManager::GetInstance()
{
	if (!m_instance)
	{
		std::lock_guard<std::mutex> guard(m_instanceMutex);
		if (!m_instance)
		{
			m_instance = new JobManager();
		}
	}

	return m_instance;
}

Job* JobManager::CreateJob()
{
	std::lock_guard<std::mutex> guard(m_JobsMutex); //DO NOT NEED THIS (just for testing)
	unsigned int idx = ++m_currentJobCount;
	Job* job = &m_Jobs[(idx - 1) % MAX_JOBS]; //(idx - 1) % MAX_JobS
	assert(job->m_WorkCounter <= 0);
	job->m_WorkCounter = 1;
	return job;
}

Job * JobManager::CreateChildJob(Job * parent)
{
	++parent->m_WorkCounter;

	Job* job = CreateJob();
	job->m_Parent = parent;
	
	return job;
}

void JobManager::FinishJob(Job * job)
{
	//std::lock_guard<std::mutex> guard(m_JobsMutex); //DO NOT NEED THIS (just for testing)
	--job->m_WorkCounter;

	if (job->m_WorkCounter == 0 && job->m_Parent != nullptr)
	{
		assert(job->m_Parent->m_WorkCounter > 0);
		//FinishJob(job->m_Parent);
		DecrementParentWorkCounter(job->m_Parent);
		job->m_Parent = nullptr;
	}

	++m_finishedJobs;
}

unsigned int JobManager::GetFinishedJobs() const
{
	return m_finishedJobs;
}

//Inspiration: https://blog.molecular-matters.com/2015/08/24/job-system-2-0-lock-free-work-stealing-part-1-basics/
void JobManager::WaitForJob(Job * job, int targetWorkCounter)
{
	assert(job->m_WorkCounter >= targetWorkCounter);

	while (job->m_WorkCounter > targetWorkCounter)
	{
		//Future improvement: process other jobs
		//Sleep(2);
	}
}

void JobManager::DecrementParentWorkCounter(Job * parent)
{
	--parent->m_WorkCounter;

	if (parent->m_WorkCounter == 0 && parent->m_Parent != nullptr)
	{
		assert(parent->m_Parent->m_WorkCounter > 0);
		//FinishJob(job->m_Parent);
		DecrementParentWorkCounter(parent->m_Parent);
		parent->m_Parent = nullptr;
	}
}

JobManager* JobManager::m_instance = nullptr;
std::mutex JobManager::m_instanceMutex;