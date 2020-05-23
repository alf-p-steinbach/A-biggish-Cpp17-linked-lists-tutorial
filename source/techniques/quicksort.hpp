#pragma once
#include "../data/Default_ordering_.hpp"
#include "Joinable_list_.hpp"

#include <assert.h>     // assert
#include <limits.h>     // CHAR_BIT

#include <array>        // std::array
#include <functional>   // std::invoke
#include <utility>      // std::exchange

namespace techniques {
    using
        std::array, std::string_view, std::string,
        std::invoke, std::exchange;
    using
        data::Default_ordering_;

    template< class Value, template <class> class Ordering_ >
    class Quicksort_
        : private Ordering_<Value>
    {
    public:
        using List = Joinable_list_<Value>;

    private:
        int m_flipflopping_index = 0;
        
        using Ordering_<Value>::compare;            // Sort of insane C++ rules at play.
        using Ordering_<Value>::average_of;

        void sort( List& list )
        {
            // Recursion base case: a list with n <= 1 nodes is necessarily sorted.
            if( list.is_empty_or_single_value() ) {
                return;
            }
            
            array<List, 2> parts = {};
            
            // Divide the nodes to parts so that every parts[0] value < every parts[1] value.
            {
                const string pivot = average_of( list.front(), list.back() );

                #ifdef QUICKSORT_WITH_ASSERTS
                    [[maybe_unused]] auto& v1 = list.front();
                    [[maybe_unused]] auto& vn = list.back();
                    assert( (v1 <= pivot) == (v1 <= vn) );
                    assert( (pivot <= vn) == (v1 <= vn) );
                #endif

                while( not list.is_empty() ) {
                    const int i = invoke( [&]() -> int
                    {
                        const auto cresult = compare( list.front(), pivot );
                        if( cresult < 0 ) {
                            return 0;
                        } else if( cresult > 0 ) {
                            return 1;
                        } else { // equal values
                            m_flipflopping_index = 1 - m_flipflopping_index;
                            return m_flipflopping_index;
                        }
                    } );
                        
                    parts[i].append( list.detached_front() );
                }
            }

            // Recurse:
            for( auto& part: parts ) { sort( part ); }
            
            // Join the now sorted 2 parts in sorted order:
            list = joined( move( parts[0] ), move( parts[1] ) );
        }
        
    public:
        Quicksort_( List& list )
        {
            sort( list );
        }
    };

    template< class Value >
    inline void quicksort( Joinable_list_<Value>& list )
    {
        (void) Quicksort_<Value, Default_ordering_>( list );
    }

    template< template <class> class Ordering_, class Value >
    inline void quicksort_( Joinable_list_<Value>& list )
    {
        (void) Quicksort_<Value, Ordering_>( list );
    }

}  // namespace techniques
