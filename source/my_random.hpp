#pragma once
// Based on
// <url: https://github.com/alf-p-steinbach/cppx-core/blob/master/source/cppx-core/
// stdlib-wrappers/random-numbers-util.hpp>

#include <limits.h>         // CHAR_BIT

#include <type_traits>      // std::(conditional_t, is_same_v, is_unsigned_v)
#include <utility>          // std::exchange
    
#ifdef __GNUC__
#   ifndef NO_WARNING_ABOUT_RANDOM_DEVICE_PLEASE
#       ifndef  _GLIBCXX_USE_RANDOM_TR1
#           pragma GCC warning \
"_GLIBCXX_USE_RANDOM_TR1 is not defined: `std::random_device` may use a fixed sequence."
#       endif
#   endif
#endif
#include <random>           // std::*

namespace my_random {
    using 
        std::conditional_t, std::is_same_v, std::is_unsigned_v,
        std::exchange,
        std::mt19937_64, std::mt19937, std::random_device,
        std::uniform_int_distribution,
        std::uniform_real_distribution;

    constexpr int   bits_per_byte               = CHAR_BIT;
    template< class T > constexpr int bits_per_ = bits_per_byte*sizeof( T );
    
    constexpr bool  system_is_64_bit_or_more    = (bits_per_<void*> >= 64);

    // A more well-defined, more reliable alternative to std::default_random_engine.
    using Generator     = conditional_t< system_is_64_bit_or_more,
        std::mt19937_64,            // Efficient for 64-bit systems.
        std::mt19937                // Efficient for 32-bit systems.
        >;
    using Bits_value    = Generator::result_type;

    static_assert( is_unsigned_v<Bits_value> );
    static_assert( sizeof( Bits_value ) >= sizeof( unsigned ) );
    static_assert( is_same_v<random_device::result_type, unsigned> );

    inline auto hardware_entropy()
        -> unsigned
    {
        static random_device the_entropy_source;
        return the_entropy_source();
    }

    class Seed
    {
        Bits_value  m_value;

    public:
        auto value() const -> Bits_value { return m_value; }

        Seed(): m_value( 0 ) {}
        explicit Seed( const Bits_value value = 0 ): m_value( value ) {}
    };

    static inline auto random_seed()
        -> Seed
    {
        constexpr auto div_up = []( size_t a, size_t b ) { return (a + b - 1)/b; };

        // Not using std::independent_bits_engine because it copies the underlying engine.
        Bits_value value = 0;
        constexpr int n_chunks = int( div_up( sizeof( Bits_value ), sizeof( unsigned ) ) );
        static_assert( n_chunks >= 1 );
        for( int i = 1; true; ++i ) {
            value |= hardware_entropy();
            if( i == n_chunks ) {
                break;
            }
            value <<= bits_per_<unsigned>;
        }
        return Seed( value );
    }

    class Bits
    {
        Generator       m_bits_generator;

    public:
        auto generator()
            -> Generator&       // Not the same type as for the general case. Same meta-type.
        { return m_bits_generator; }

        auto next()
            -> Bits_value
        { return m_bits_generator(); }

        explicit Bits( const Seed seed = random_seed() ):
            m_bits_generator( seed.value() )
        {}
    };

    class Choices
    {
        Bits            m_bits;
        Bits_value      m_value;
        int             m_n_bits_consumed;
    
    public:
        auto generator()
            -> Generator&
        { return m_bits.generator(); }

        auto next()
            -> bool
        {
            if( m_n_bits_consumed == bits_per_<Bits_value> ) {
                m_value = m_bits.next();
                m_n_bits_consumed = 0;
            }
            ++m_n_bits_consumed;
            return !!(exchange( m_value, m_value/2 ) % 2);
        }
        
        explicit Choices( const Seed seed = random_seed() ):
            m_bits( seed ),
            m_value( 0 ),
            m_n_bits_consumed( bits_per_<Bits_value> )
        {}
    };

    template< class Integer >
    class Integers_
    {
        using Adapter = uniform_int_distribution<Integer>;

        Generator       m_bits_generator;
        Adapter         m_adapt;

    public:
        auto generator()
            -> Generator&
        { return m_bits_generator; }

        auto next()
            -> Integer
        { return m_adapt( m_bits_generator ); }

        explicit Integers_( const Integer n_unique_values, const Seed seed = random_seed() ):
            m_bits_generator( seed.value() ),
            m_adapt( 0, n_unique_values - 1 )
        {}
    };

    template< class Integer = int >
    struct Integer_
    {
        static auto from( Generator& g, const Integer n_unique_values )
            -> Integer
        { return uniform_int_distribution<Integer>( 0, n_unique_values - 1 )( g ); }
    };

    template< class Number >        // For floating point numbers, e.g. `double`.
    class Numbers_
    {
        using Adapter = std::uniform_real_distribution<Number>;

        Generator       m_bits_generator;
        Adapter         m_adapt;

    public:
        auto generator()
            -> Generator&
        { return m_bits_generator; }

        auto next()
            -> Number
        { return m_adapt( m_bits_generator ); }

        explicit Numbers_( const Seed seed = random_seed() ):
            m_bits_generator( seed.value() )
        {}
    };

    template< class Number >
    struct Number_
    {
        static auto from( Generator& g )
            -> Number
        { return std::uniform_real_distribution<Number>()( g ); }
    };

    using Integers  = Integers_<int>;
    using Integer   = Integer_<int>;
    using Numbers   = Numbers_<double>;
    using Number    = Number_<double>;

}  // namespace my_random
