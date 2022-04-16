#pragma once
#include <memory>
#include <chrono>

class BenchmarkTimer
{
public:
	BenchmarkTimer();
	~BenchmarkTimer();
	void Stop();
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> StartTime;
};