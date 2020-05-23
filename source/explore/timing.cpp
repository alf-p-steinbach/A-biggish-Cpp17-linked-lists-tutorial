#include "../util/timing.hpp"
using util::Timer;

#include <stdint.h>

#include <iostream>
using std::cout, std::endl;

auto fib( const int x )
    -> int64_t
{ return (x <= 1? 1 : fib( x - 1 ) + fib( x - 2 )); }

auto main() -> int
{
    Timer timer;
    const int64_t result = fib( 47 );
    const double n_seconds = timer.elapsed_seconds();
    
    cout << "fib(47) = " << result << ", computed in " << n_seconds << " seconds." << endl;
}
