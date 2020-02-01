#pragma once
#include "List.hpp"

#include <assert.h>     // assert

#include <array>        // std::array
#include <functional>   // std::invoke
#include <utility>      // std::move

namespace oneway_sorting_examples {
    using std::array, std::invoke, std::move, std::string_view;

    struct Merge_sort_input
    {
        string_view         previous_value = "";
        array<List, 2>      in;
        
        auto next()
            -> Node*
        {
            const int n_empty = (not in[0].head + not in[1].head);
            if( n_empty == 2 ) {
                return nullptr;
            } else if( n_empty == 1 ) {
                return unlinked( in[0].head? in[0].head : in[1].head );
            } else { // n_empty == 0
                const array<string_view, 2> v =
                {
                    in[0].head->value, in[1].head->value
                };

                if( (v[0] >= previous_value) == (v[1] >= previous_value) ) {
                    return unlinked( v[0] < v[1]? in[0].head : in[1].head );
                } else if( v[0] >= previous_value ) {
                    return unlinked( in[0].head );
                } else {
                    return unlinked( in[1].head );
                }
            }
        }
    };

    inline auto merge_sort_iteratively( List& list )
        -> int      // Number of passes used.
    {
        if( not list.head or not list.head->next ) {
            return 0;
        }

        Merge_sort_input    input;
        array<List, 2>      out;

        out[0] = move( list );
        for( int n_passes = 1; true; ++n_passes ) {
            input.previous_value = "";
            input.in[0] = move( out[0] );
            input.in[1] = move( out[1] );
            array<List::Appender, 2> out_appenders = {out[0].head, out[1].head};

            int i_out = 0;
            for( ;; ) {
                const Type_<Node*> p = input.next();
                if( not p ) {
                    break;          // Merge finished.
                } else if( p->value < input.previous_value ) {
                    i_out = 1 - i_out;      // Start a new run in the other out-list.
                }
                input.previous_value = p->value;
                out_appenders[i_out].append( p );
            }

            if( not out[0].head or not out[1].head ) {
                list = move( out[0].head? out[0] : out[1] );
                return n_passes;
            }
        }
    }

}  // namespace oneway_sorting_examples
