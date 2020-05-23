#pragma once
#include "../std_chrono.hpp"
#include "Timer_ticks.hpp"

#include <stdint.h>         // int32_t, int64_t

namespace util::timing {
    namespace _alias { namespace timing = util::timing; }

    class Timer
    {
        volatile Timer_ticks::Value     m_start_time;
        
    public:
        Timer() noexcept:
            m_start_time( Timer_ticks::now().count() )
        {}

        auto start_time() const
            -> Timer_ticks
        { return m_start_time; }

        auto elapsed_ticks() const noexcept
            -> Timer_ticks
        { return Timer_ticks::now() - start_time(); }
        
        auto elapsed_seconds() const noexcept
            -> double
        { return elapsed_ticks().as_seconds(); }
    };
}  // namespace util::timing
