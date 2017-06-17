#pragma once

class Game;

class ExpensiveSystemY
{
public:
	ExpensiveSystemY();
	~ExpensiveSystemY();

	Game* m_Game;

	int Update();

	//DEBUG:
	static std::atomic_uint m_JobExecCount;
};

