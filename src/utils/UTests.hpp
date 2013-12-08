#ifndef UTESTS_H
#define	UTESTS_H

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)

namespace win32 {
    #include <windows.h>
}

class timer
{
private:
    win32::LARGE_INTEGER start_time_;
public:
    timer();
    void restart();
    double elapsed() const;
};

#else

extern "C"
{
	#include <sys/time.h>
}

class timer
{
private:
    timeval _start_time;
public:
    timer();
    void restart();
    double elapsed() const;
};

#endif

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
using namespace win32;
#endif

#include <iostream>

template <typename Func>
void uTest_Time(Func f, int loop, const char* label)
{
	double temp, total = 0, highest = 0;
	timer t;
	for(int i = 0; i < loop; ++i)
	{
		t.restart();
		f();
		temp = t.elapsed();
		total += temp;
		if(temp > highest)
		{
			highest = temp;
		}
	}
	
	std::cout.flags(std::ios::left);
	std::cout.width(20);
	std::cout << label;
	std::cout.setf(std::ios::fixed);
	std::cout.precision(4);
	std::cout << " - Av: " << (total / loop) * 1000 << "ms. | Hi: " << (highest * 1000) << "ms. | T: " << (total * 1000) << "ms. (" << loop << "x)\n";
}
template <typename F1, typename F2>
void uTest_Time(F1 f1, F2 f2, int loop, const char* label)
{
	double temp, total = 0, highest = 0;
	timer t;
	for(int i = 0; i < loop; ++i)
	{
		f1();
		t.restart();
		f2();
		temp = t.elapsed();
		total += temp;
		if(temp > highest)
		{
			highest = temp;
		}
	}
	
	std::cout.flags(std::ios::left);
	std::cout.width(20);
	std::cout << label;
	std::cout.setf(std::ios::fixed);
	std::cout.precision(4);
	std::cout << " - Av: " << (total / loop) * 1000 << "ms. | Hi: " << (highest * 1000) << "ms. | T: " << (total * 1000) << "ms. (" << loop << "x)\n";
}

#define uTest_Init int uTest_Failed = 0;
#define uTest_True(e, l) std::cout.flags(std::ios::left); std::cout.width(23); std::cout << l; if(e){ std::cout << "Passed"; } else { std::cout << "Failed"; ++uTest_Failed; } cout << "\n";
#define uTest_False(e, l) uTest_True(!(e), l);
#define uTest_Abort(l) if(uTest_Failed > 0){ cout << "\nOne of the tests from \"Test " << l << " Set\" has failed, aborting further tests...\n(Failed tests: " << uTest_Failed << ")\n"; return 1; }
#define uTest_Finish if(uTest_Failed == 0){ cout << "\nAll tests passed!\n"; } else { cout << "\nSome tests failed!\n(Failed tests: " << uTest_Failed << ")\n"; } return 1;

#endif	/* UTESTS_H */

