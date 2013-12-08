#include "UTests.h"

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

timer::timer()
{
	QueryPerformanceCounter(&start_time_);
}
void timer::restart()
{
	QueryPerformanceCounter(&start_time_);
}
double timer::elapsed() const
{
	win32::LARGE_INTEGER end_time, frequency;
	QueryPerformanceCounter(&end_time);
	QueryPerformanceFrequency(&frequency);
	return double(end_time.QuadPart - start_time_.QuadPart) / frequency.QuadPart;
}

#else

extern "C"
{
	#include <sys/time.h>
}

timer::timer()
{
	gettimeofday(&_start_time, NULL);
}
void timer::restart()
{
	gettimeofday(&_start_time, NULL);
}
double timer::elapsed() const
{
	timeval now;
	gettimeofday(&now, NULL);
	return (now.tv_sec + ((double)now.tv_usec / 1000000)) - (_start_time.tv_sec + ((double)_start_time.tv_usec / 1000000));
}

#endif

