#pragma once

/*Inspiration: 
Game Programming Gems 8
Intel: nulstein
https://blog.molecular-matters.com/2015/08/24/job-system-2-0-lock-free-work-stealing-part-1-basics/

*/
struct Job;

typedef void(*JobCallback)(Job*);

struct Job
{
	Job()
		: m_Data(nullptr), m_Parent(nullptr), m_WorkCounter(0)
	{}

	void* m_Data;
	JobCallback m_Callback;
	Job* m_Parent;
	std::atomic_int m_WorkCounter;
};

