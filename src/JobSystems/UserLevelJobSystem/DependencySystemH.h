#pragma once

class Game;

class DependencySystemH
{
public:
	DependencySystemH();
	~DependencySystemH();

	Game* m_Game;

	int Update();

	//DEBUG:
	static std::atomic_uint m_JobExecCount;
};

