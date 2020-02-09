#pragma once
// Based on
// <url: https://github.com/alf-p-steinbach/cppx-core/blob/master/source/cppx-core/
// stdlib-wrappers/chrono-util.hpp>

#include <stdint.h>         // int64_t

#include <chrono>           // std::chrono::*
#include <ratio>            // std::(milli, micro, nano)
#include <type_traits>      // std::conditional

namespace my_chrono {
    namespace chrono = std::chrono;
    using std::conditional_t, std::milli, std::micro, std::nano;

    using Timer_clock = conditional_t<chrono::high_resolution_clock::is_steady,
        chrono::high_resolution_clock,
        chrono::steady_clock
        >;
    using Time_point    = Timer_clock::time_point;  // Result of static member `now()`.
    using Duration      = Timer_clock::duration;    // Difference type of time points.

    template< class Rep, class Period >
    inline auto as_seconds( const chrono::duration<Rep, Period> duration_value )
        -> double
    { return chrono::duration<double>( duration_value ).count(); }

    template< class Rep, class Period >
    inline auto as_milliseconds( const chrono::duration<Rep, Period> duration_value )
        -> double
    { return chrono::duration<double, milli>( duration_value ).count(); }

    template< class Rep, class Period >
    inline auto as_microseconds( const chrono::duration<Rep, Period> duration_value )
        -> double
    { return chrono::duration<double, micro>( duration_value ).count(); }

    template< class Rep, class Period >
    inline auto as_nanoseconds( const chrono::duration<Rep, Period> duration_value )
        -> double
    { return chrono::duration<double, nano>( duration_value ).count(); }

    inline auto min_of( const Duration& a, const Duration& b )
        -> Duration
    { return (a < b? a : b); }

    inline auto max_of( const Duration& a, const Duration& b )
        -> Duration
    { return (a > b? a : b); }

    struct Measurement
    {
        const Duration      duration;
        const int64_t       n_iterations;
        
        auto average_seconds() const
            -> double
        { return as_seconds( duration )/n_iterations; }
    };

    template< class Func >
    inline auto time_per( const Func& f )
        -> Measurement
    {
        Duration time_a;
        int64_t n = 1;
        for( ;; ) {
            const Time_point a_start = Timer_clock::now();
            for( int64_t i = 1; i <= n; ++i ) {
                f();
            }
            time_a = Timer_clock::now() - a_start;
            if( time_a > Duration::zero() ) {
                break;
            }
            n *= 2;
        }

        Duration time_b = time_a;
        while( time_b < min_of( chrono::seconds( 1 ), 1000*time_a ) ) {
            n *= 2;
            Time_point b_start = Timer_clock::now();
            for( int64_t i = 1; i <= n; ++i ) {
                f();
            }
            time_b = Timer_clock::now() - b_start;
        }
        return Measurement{ time_b, n };
    }

}  // namespace my_chrono
