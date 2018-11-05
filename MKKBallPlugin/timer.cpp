#include "timer.h"


Timer::Timer(double seconds)
{
	duration = seconds;
}

void Timer::start()
{
	//SET the clock to now
	timer_start = std::clock();
}

//DECREPATED
void Timer::reset()
{
	start();
}

bool Timer::is_alarmed()
{
	//The time elapsed
	double elapsed;
	std::clock_t now = std::clock();

	elapsed = (now - timer_start) / (double)CLOCKS_PER_SEC;

	return (elapsed > duration);
}
