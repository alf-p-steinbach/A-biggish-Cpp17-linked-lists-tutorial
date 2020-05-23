#pragma once
#include "../std_chrono.hpp"
#include "../Number_like_.hpp"

#include <stdint.h>         // int32_t, int64_t

namespace util::timing {
    class Timer_ticks:
        public Number_like_<Timer_ticks>
    {
    public:
        using Value = int64_t;
        
    private:
        Value   m_value;

        // System specific functions:
        static inline auto get_ticks_per_second() noexcept -> Value;
        static inline auto get_ticks_now() noexcept -> Value;
        
    public:
        Timer_ticks( const Value value ) noexcept:
            m_value( value )
        {}

        static auto now() noexcept
            -> Timer_ticks
        { return Timer_ticks( get_ticks_now() ); }

        static auto per_second() noexcept
            -> Value
        {
            static const Value the_value = get_ticks_per_second();
            return the_value;
        }

        auto count() const noexcept -> Value { return m_value; }
        auto as_seconds() const noexcept -> double { return 1.0*m_value/per_second(); }

        auto numerical() const noexcept -> Value { return m_value; }
    };

    #if defined( FORCE_STD_CHRONO_TIMER_IMPLEMENTATION ) or not defined( _WIN32 )
        inline auto Timer_ticks::get_ticks_per_second() noexcept
            -> Timer_ticks::Value
        {
            using std::ratio_divide, std::ratio;
            using std_chrono::Duration;

            using Freq = ratio_divide<ratio<1>, Duration::period>;
            static_assert( Freq::den == 1 );

            return static_cast<Timer_ticks::Value>( Freq::num );
        }

        inline auto Timer_ticks::get_ticks_now() noexcept
            -> Timer_ticks::Value
        {
            return static_cast<Timer_ticks::Value>(
                std_chrono::Timer_clock::now().time_since_epoch().count()
                );
        }
    #else
        namespace winapi {
            using BOOL          = int;
            using DWORD         = uint32_t;
            using LONG          = int32_t;
            using LONGLONG      = int64_t;

            union LARGE_INTEGER
            {
                struct{ DWORD LowPart; LONG  HighPart; } u;
                LONGLONG QuadPart;
            };

            extern "C"
            auto __stdcall QueryPerformanceCounter( LARGE_INTEGER* ) -> BOOL;

            extern "C"
            auto __stdcall QueryPerformanceFrequency( LARGE_INTEGER* ) -> BOOL;
        }  // namespace winapi
        
        inline auto Timer_ticks::get_ticks_per_second() noexcept
            -> Timer_ticks::Value
        {
            winapi::LARGE_INTEGER result;
            winapi::QueryPerformanceFrequency( &result );
            return static_cast<Timer_ticks::Value>( result.QuadPart );
        }

        inline auto Timer_ticks::get_ticks_now() noexcept
            -> Timer_ticks::Value
        {
            winapi::LARGE_INTEGER result;
            winapi::QueryPerformanceCounter( &result );
            return static_cast<Timer_ticks::Value>( result.QuadPart );
        }
    #endif

}  // namespace util::timing
