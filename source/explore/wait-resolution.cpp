#include "../my_chrono.hpp"
using my_chrono::Timer_clock, my_chrono::Duration, my_chrono::as_seconds;
namespace chrono = std::chrono;

#include <algorithm>        // std::max
#include <type_traits>      // std::is_same_v
#include <iomanip>          // std::setw
#include <iostream>         // std::(cout, endl)
#include <thread>           // std::this_thread::*
using 
    std::min, std::max, std::is_same_v,
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
        const auto direct_start_time = Timer_clock::now();
        this_thread::sleep_for( resolution );
        const auto direct_end_time = Timer_clock::now();
        
        const int n = min( 1000, max( 1,
            static_cast<int>( chrono::milliseconds( 100 )/resolution )
            ) );
        const auto loop_start_time = Timer_clock::now();
        for( int i = 1; i <= n; ++i ) {
            this_thread::sleep_for( resolution );
        }
        const auto loop_end_time = Timer_clock::now();
        
        cout
            << "Interval " << as_seconds( resolution ) << "s"
            << " measured directly = " << as_seconds( direct_end_time - direct_start_time )
            << ", average of " << setw( 5 ) << right << n << " = "
            << as_seconds( loop_end_time - loop_start_time )/n
            << "." << endl;
    }
}
