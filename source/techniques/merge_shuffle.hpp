#pragma once
#include "Joinable_list_.hpp"
#include "../my_random.hpp"

#include <array>        // std::array
#include <utility>      // std::forward

namespace techniques {
    using std::array, std::forward;
    using my_random::Choices, my_random::Seed, my_random::random_seed;

    template< class Value_type >
    struct Merge_shuffle_
    {
        using List = Joinable_list_<Value_type>;

        static void shuffle( List& list, Choices& choices )
        {
            // Base case: a list with n <= 1 nodes is necessarily randomly ordered.
            if( list.is_empty_or_single_value() ) {
                return;
            }
            array<List, 2> parts;
            
            // Divide the nodes about equally to part lists (a partitioning of nodes):
            for( int i = 0; not list.is_empty(); ) {
                parts[i%2].append( list.detached_front() );
                ++i;
            }
            
            // Recurse:
            for( int i = 0; i < 2; ++i ) {
                shuffle( parts[i], choices );
            }
            
            // Merge the now random 2 parts randomly:
            for( ;; ) {
                const int n_empty = parts[0].is_empty() + parts[1].is_empty();
                if( n_empty == 2 ) {
                    break;      // Hurray, we're finished at this recursion level.
                } else if( n_empty == 1 ) {
                    const int i_rest = (parts[0].is_empty()? 1 : 0);
                    list.append( move( parts[i_rest] ) );
                } else { // n_empty == 0
                    const int i_random = choices.next();
                    list.append( parts[i_random].detached_front() );
                }
            }
        }

        static void shuffle( List& list, const Seed seq_nr = random_seed() )
        {
            Choices choices( seq_nr );
            shuffle( list, choices );
        }
    };

    template< class Value_type, class... Args >
    inline void merge_shuffle( Joinable_list_<Value_type>& list, Args&&... args )
    {
        Merge_shuffle_<Value_type>::shuffle( list, forward<Args>( args )... );
    }

}  // namespace oneway_sorting_examples
