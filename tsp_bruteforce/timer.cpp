#include "timer.h"

void timer::start()
{
	start_time_ = high_resolution_clock::now();
	
}

void timer::stop()
{
	stop_time_ = high_resolution_clock::now();

}

double timer::elapsed_time()
{
	return duration_cast<duration<double>>(stop_time_ - start_time_).count();
	
}