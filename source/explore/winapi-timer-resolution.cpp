#define NOMINMAX
#include <windows.h>    // LARGE_INTEGER, QueryPerformanceFrequency

#include <stdlib.h>     // EXIT_...
#include <iostream>     // std::(cout, endl)
using std::cout, std::endl;

auto main()
    -> int
{
    LARGE_INTEGER timer_frequency;
    const bool ok = !!QueryPerformanceFrequency( &timer_frequency );
    if( not ok ) {
        return EXIT_FAILURE;
    }
    const double tick_duration = 1.0/timer_frequency.QuadPart;
    cout << "For this process each tick is " << tick_duration << " seconds." << endl;
}
