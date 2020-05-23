#include "../util/std_chrono.hpp"
using namespace util::std_chrono::_alias;
using   std_chrono::Timer_clock, std_chrono::Time_point, std_chrono::as_seconds;

#include "shuffled_english_words_list.hpp"
#include "merge_sort_iteratively.hpp"
namespace x = oneway_sorting_examples;
using   x::english_words_list, x::shuffled_english_words_list,
        x::Node, x::List, x::merge_sort_iteratively;

#include <iomanip>          // std::setw
#include <iostream>         // std::(fixed, cout, clog, endl)    
#include <limits>           // std::numeric_limits
using   std::setw, std::numeric_limits,
        std::fixed, std::cout, std::clog, std::endl;
    
constexpr double nan_value = numeric_limits<double>::quiet_NaN();

auto seconds_for( List words )
    -> double
{
    const Time_point    start_time  = Timer_clock::now();
    merge_sort_iteratively( words );
    const Time_point    end_time    = Timer_clock::now();

    return (words.is_sorted()? as_seconds( end_time - start_time ) : nan_value);
}

auto main()
    -> int
{
    const auto w = setw( 16 );
    cout << fixed;

    cout    << "Iterative \"natural runs\" merge-sort results in seconds, for "
            << english_words_list().count() << " words:"
            << endl;
    cout << endl;
    cout << w << "Sorted data:" << w << "Shuffled data:" << w << "Diff:" << endl;
    for( int i = 1; i <= 12; ++i ) {
        const double sorted_time    = seconds_for( english_words_list() );
        const double shuffled_time  = seconds_for( shuffled_english_words_list() );
        const double diff           = shuffled_time - sorted_time;
        cout << w << sorted_time << w << shuffled_time << w << diff << endl;
    }
}
