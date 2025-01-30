#pragma once

#include "curses.h"

class Application
{
public:
	Application();
	~Application();

	void Init();
	void Run();
private:
	bool m_Running = true;
};

