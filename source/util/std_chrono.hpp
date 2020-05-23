#pragma once
// Based on
// <url: https://github.com/alf-p-steinbach/cppx-core/blob/master/source/cppx-core/
// stdlib-wrappers/chrono-util.hpp>

#include <stdint.h>         // int64_t

#include <chrono>           // std::chrono::*
#include <ratio>            // std::(milli, micro, nano)
#include <type_traits>      // std::conditional

namespace util::std_chrono {
    namespace _alias { namespace std_chrono = util::std_chrono; }

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

}  // namespace my_chrono
