#pragma once
#include <chrono>

using namespace std::chrono;

class timer
{
public:
	void start();
	void stop();

	double elapsed_time();

private:
	high_resolution_clock::time_point start_time_;
	high_resolution_clock::time_point stop_time_;
};