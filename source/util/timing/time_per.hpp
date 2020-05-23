#pragma once
#include "Timer.hpp"

#include <stdint.h>         // int32_t, int64_t

namespace util::timing {
    namespace _alias { namespace timing = util::timing; }

    struct Measurement
    {
        const Timer_ticks   duration;
        const int64_t       n_iterations;
        
        auto average_seconds() const
            -> double
        { return duration.as_seconds()/n_iterations; }
    };

    template< class Func >
    inline auto time_per( const Func& f )
        -> Measurement
    {
        Timer_ticks time_a;
        int64_t n = 1;
        for( ;; ) {
            Timer timer;
            for( int64_t i = 1; i <= n; ++i ) {
                f();
            }
            time_a = timer.elapsed_ticks();
            using namespace std::rel_ops;
            if( time_a > Timer_ticks( 0 ) ) {   // Explicit conversion for g++ compiler.
                break;
            }
            n *= 2;
        }

        Timer_ticks time_b = time_a;
        const Timer_ticks time_b_limit = min( Timer_ticks::per_second(), 1000*time_a );
        while( time_b < time_b_limit ) {
            n *= 2;
            Timer timer;
            for( int64_t i = 1; i <= n; ++i ) {
                f();
            }
            time_b = timer.elapsed_ticks();
        }
        return Measurement{ time_b, n };
    }
}  // namespace util::timing
