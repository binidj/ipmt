#include "BenchmarkTimer.h"

BenchmarkTimer::BenchmarkTimer()
{
	StartTime = std::chrono::high_resolution_clock::now();
}

BenchmarkTimer::~BenchmarkTimer()
{
	Stop();
}

void BenchmarkTimer::Stop()
{
	auto EndTime = std::chrono::high_resolution_clock::now();

	long long StartCount = std::chrono::time_point_cast<std::chrono::microseconds>(StartTime).time_since_epoch().count();
	long long EndCount = std::chrono::time_point_cast<std::chrono::microseconds>(EndTime).time_since_epoch().count();

	long long Duration = EndCount - StartCount;
	double Milliseconds = Duration * 0.001;

	printf("Code took %lld us / %lf ms\n", Duration, Milliseconds);
}
