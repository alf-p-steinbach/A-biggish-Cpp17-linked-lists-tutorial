#include "../my_chrono.hpp"
using
    my_chrono::Timer_clock, my_chrono::Duration, my_chrono::as_seconds,
    my_chrono::Measurement, my_chrono::time_per;
namespace chrono = std::chrono;

#include <algorithm>        // std::max
#include <type_traits>      // std::is_same_v
#include <iomanip>          // std::setw
#include <iostream>         // std::(cout, endl)
#include <thread>           // std::this_thread::*
using 
    std::is_same_v,
    std::fixed, std::left, std::right, std::setw, std::setprecision,
    std::cout, std::endl;
namespace this_thread = std::this_thread;

auto main()
    -> int
{
    const bool using_steady_clock = is_same_v<Timer_clock, chrono::steady_clock>;
    const auto clock_name = (using_steady_clock? "steady_clock" : "high_resolution_clock");
    cout << "Using std::" << clock_name << "." << endl;
    
    cout << fixed << setprecision( 12 );
    for(    Duration resolution = chrono::seconds( 1 );
            resolution >= chrono::microseconds( 1 );
            resolution /= 2 ) {
        const Measurement m = time_per( [&]{ this_thread::sleep_for( resolution ); } );
        cout
            << "Interval " << as_seconds( resolution ) << " secs"
            << " measured as " << m.average_seconds()
            << " using " << setw( 5 ) << m.n_iterations << " iterations."
            << endl;
    }
}
