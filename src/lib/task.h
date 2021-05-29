#ifndef _H_task_
#define _H_task_

#include <thread>

struct task
{
	
	void (*tick)(task*) = NULL;
	std::thread thread;
	volatile unsigned int status = 1;
	bool auto_join;

	task(void) {};
	task(void (*func)(task*)) : tick(func) {};

	void bind(void (*func)(task*))
	{
		tick = func;
	};

	void update(void)
	{
		status = 1;
		thread = std::thread(tick, this);
	};

	~task()
	{
		if (thread.joinable())
			thread.join();
	}
	
};

#endif