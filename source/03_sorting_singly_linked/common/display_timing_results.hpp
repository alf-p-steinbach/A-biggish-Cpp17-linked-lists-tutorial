#pragma once
#include "../../util/calc.hpp"
#include "../../util/timing.hpp"

#include "../shuffled_english_words_list.hpp"

#include <iomanip>          // std::setw
#include <iostream>         // std::(fixed, cout, clog, endl)
#include <functional>       // std::function
#include <string_view>      // std::string_view

namespace oneway_sorting_examples::common
{
    namespace _alias { namespace common = oneway_sorting_examples::common; }

    using   namespace util::calc::_alias;
    using   namespace util::timing::_alias;
    using   calc::percent;
    using   timing::Timer;
    
    namespace x = oneway_sorting_examples;
    using   data::n_english_words;
    using   x::english_words_list, x::shuffled_english_words_list, x::List;
    
    using   std::setw, std::fixed, std::cout, std::endl,
            std::function,
            std::string_view;

    using Sort_function = function< void(List&) >;

    auto seconds_for( const Sort_function& sort, List&& words )
        -> double
    {
        Timer timer;
        sort( words );
        const double seconds = timer.elapsed_seconds();

        return (words.is_sorted()? seconds : calc::nan);
    }

    void display_timing_results( const string_view& algorithm_name, const Sort_function& sort )
    {
        cout    << algorithm_name << " results in seconds, for "
                << n_english_words << " words:" << endl;
        cout    << endl;

        const auto w = setw( 16 );
        cout << fixed;
        cout << w << "Sorted data:" << w << "Shuffled data:" << w << "% difference:" << endl;
        for( int i = 1; i <= 12; ++i ) {
            const double time_for_sorted    = seconds_for( sort, english_words_list() );
            const double time_for_shuffled  = seconds_for( sort, shuffled_english_words_list() );
            const double pct_diff           = percent( time_for_sorted, time_for_shuffled );
            cout << w << time_for_sorted << w << time_for_shuffled << w << pct_diff << endl;
        }
    }
}  // namespace oneway_sorting_examples::common
