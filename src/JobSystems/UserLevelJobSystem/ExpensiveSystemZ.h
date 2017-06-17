#pragma once

class Game;

class ExpensiveSystemZ
{
public:
	ExpensiveSystemZ();
	~ExpensiveSystemZ();

	Game* m_Game;

	int Update();

	//DEBUG:
	static std::atomic_uint m_JobExecCount;
};

