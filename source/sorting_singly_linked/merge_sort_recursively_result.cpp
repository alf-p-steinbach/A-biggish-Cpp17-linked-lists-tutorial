#include "../my_chrono.hpp"
#include "../my_random.hpp"
using my_chrono::Timer_clock, my_chrono::Time_point, my_chrono::as_seconds;

#include "shuffled_english_words_list.hpp"
#include "merge_sort_recursively.hpp"
namespace x = oneway_sorting_examples;
using
    x::english_words_list, x::shuffled_english_words_list,
    x::Node, x::List, x::merge_sort_recursively;
using Words_list_func = auto()->List;

#include <iomanip>          // std::setw
#include <iostream>         // std::(fixed, cout, clog, endl)    
#include <limits>           // std::numeric_limits
#include <optional>         // std::optional
using
    std::setw, std::numeric_limits,
    std::fixed, std::cout, std::clog, std::endl,
    std::optional;

auto seconds_for( Words_list_func& words_list )
    -> optional<double>
{
    List                words       = words_list();
    const Time_point    start_time  = Timer_clock::now();
    merge_sort_recursively( words );
    const Time_point    end_time    = Timer_clock::now();

    if( not words.is_sorted() ) {
        return {};
    }
    return as_seconds( end_time - start_time );
}

auto main()
    -> int
{
    cout << fixed;

    cout    << "Recursive merge-sort results in seconds, for "
            << english_words_list().count() << " words:"
            << endl;
    cout << endl;
    const auto w = setw( 16 );
    cout << w << "Sorted data:" << w << "Shuffled data:" << w << "Diff:" << endl;
    for( int i = 1; i <= 12; ++i ) {
        constexpr double nan = numeric_limits<double>::quiet_NaN();
        using F = Words_list_func*;
        const auto& sorted_words    = *english_words_list;
        const auto& shuffled_words  = *F( []{ return shuffled_english_words_list(); } );

        const double sorted_time    = seconds_for( sorted_words ).value_or( nan );
        const double shuffled_time  = seconds_for( shuffled_words ).value_or( nan );
        cout
            << w << sorted_time
            << w << shuffled_time
            << w << shuffled_time - sorted_time
            << endl;
    }
}
