#pragma once
#include <ctime>

class Timer {
public:
	Timer(double seconds = 1);
	void start();
	void reset();
	bool is_alarmed();

private:
	double duration;
	std::clock_t timer_start;

	

};