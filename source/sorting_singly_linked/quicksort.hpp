#pragma once
#include "List.hpp"

#include <assert.h>     // assert
#include <limits.h>     // CHAR_BIT

#include <algorithm>    // std::max
#include <array>        // std::array
#include <functional>   // std::invoke
#include <utility>      // std::exchange

namespace oneway_sorting_examples {
    using std::invoke, std::max, std::array, std::exchange, std::string_view, std::string;
    
    class Quicksort
    {
        int     m_flipflopping_index = 0;

        inline static auto average_of( const string_view& a, const string_view& b )
            -> string;

        struct Joinable_list;
        inline void sort( Joinable_list& list );
        
    public:
        inline Quicksort( List& list );
    };

    inline void quicksort( List& list )
    {
        (void) Quicksort( list );
    }

    inline auto Quicksort::average_of( const string_view& a, const string_view& b )
        -> string
    {
        using Byte = unsigned char;
        constexpr int bits_per_byte = CHAR_BIT;

        string result;
        const int n_codes = static_cast<int>( max( a.length(), b.length() ) );
        result.resize( n_codes + 1 );
        
        // result = a + b;
        unsigned carry = 0;
        for( int i = n_codes - 1;  i >= 0; --i ) {
            const unsigned code_a = (i >= int( a.size() )? 0u : Byte( a[i] ));
            const unsigned code_b = (i >= int( b.size() )? 0u : Byte( b[i] ));
            const unsigned sum = code_a + code_b + carry;
            result[i] = char( sum & Byte( -1 ) );
            carry = sum >> bits_per_byte;
        }
        
        // result /= 2;
        for( int i = 0; i < n_codes + 1; ++i ) {
            const int new_msb = carry;
            carry = Byte( result[i] )%2;
            result[i] = char( Byte( result[i] )/2 + (new_msb << (bits_per_byte - 1)) );
        }
        if( result[n_codes] == 0 ) {
            result.resize( n_codes );
        }
        return result;
    }

    struct Quicksort::Joinable_list
    {
        Node*   head;
        Node*   last;
        
        friend auto joined( Joinable_list& a, Joinable_list& b )
            -> Joinable_list
        {
            (a.head? a.last->next : a.head) = b.head;
            if( b.last ) { a.last = b.last; }
            b = Joinable_list();
            return exchange( a, Joinable_list() );
        }
    };

    void Quicksort::sort( Joinable_list& list )
    {
        // Recursion base case: a list with n <= 1 nodes is necessarily sorted.
        if( not list.head or not list.head->next ) {
            return;
        }
        
        array<Joinable_list, 2> parts = {};
        
        // Divide the nodes to parts so that every parts[0] value < every parts[1] value.
        {
            const string pivot = average_of( list.head->value, list.last->value );

            #ifdef QUICKSORT_WITH_ASSERTS
                [[maybe_unused]] auto& v1 = list.head->value;
                [[maybe_unused]] auto& vn = list.last->value;
                assert( (v1 <= pivot) == (v1 <= vn) );
                assert( (pivot <= vn) == (v1 <= vn) );
            #endif

            array<List::Appender, 2> appenders = {parts[0].head, parts[1].head};
            while( list.head ) {
                const int i = invoke( [&]() -> int
                {
                    const int cresult = list.head->value.compare( pivot );
                    if( cresult < 0 ) {
                        return 0;
                    } else if( cresult > 0 ) {
                        return 1;
                    } else { // equal values
                        m_flipflopping_index = 1 - m_flipflopping_index;
                        return m_flipflopping_index;
                    }
                } );
                    
                appenders[i].append( unlinked( list.head ) );
            }
            for( int i = 0; i < 2; ++i ) { parts[i].last = appenders[i].last(); }
            // At this point list.head == nullptr and list.last is invalid.
        }

        // Recurse:
        for( auto& part: parts ) { sort( part ); }
        
        // Join the now sorted 2 parts in sorted order:
        list = joined( parts[0], parts[1] );
    }

    inline Quicksort::Quicksort( List& list )
    {
        const auto p_last_node = List::Appender( list.head ).last();
        Joinable_list jlist{ list.head, p_last_node };
        sort( jlist );
        list.head = jlist.head;
    }
}  // namespace oneway_sorting_examples
