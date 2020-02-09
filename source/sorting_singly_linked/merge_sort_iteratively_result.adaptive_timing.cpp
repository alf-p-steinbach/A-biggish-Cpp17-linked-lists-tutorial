#include "../my_chrono.hpp"
#include "../my_random.hpp"
using my_chrono::Measurement, my_chrono::time_per;

#include "shuffled_english_words_list.hpp"
#include "merge_sort_iteratively.hpp"
namespace x = oneway_sorting_examples;
using
    x::english_words_list, x::shuffled_english_words_list,
    x::Node, x::List, x::merge_sort_iteratively;
using Words_list_func = auto()->List;

#include <iomanip>          // std::setw
#include <iostream>         // std::(fixed, cout, clog, endl)    
#include <limits>           // std::numeric_limits
#include <optional>         // std::optional
#include <stdexcept>        // std::(exception, runtime_error)
#include <string>
#include <vector>           // std::vector;
using
    std::exception, std::runtime_error, std::string, std::to_string, std::vector,
    std::numeric_limits, std::optional,
    std::setw, std::fixed, std::cout, std::clog, std::endl;

void log( const string& s )
{
    // Turn off logging output by redirecting the error stream, e.g. in Windows `a 2>nul`.
    clog << "- " << s << endl;
}
    
auto seconds_for( Words_list_func& words_list )
    -> optional<double>
{
    log( "Preparing data." );
    vector<List> words( 2049, words_list() );
    const int n_lists = words.size();
    int n_sorted = 0;

    log( "Measuring" );
    const Measurement   m           = time_per( [&]
    {
        if( n_sorted == n_lists ) { throw runtime_error( "Too few prepared lists." ); }
        merge_sort_iteratively( words[n_sorted] );
        ++n_sorted;
    } );
    log( "Used " + to_string( m.n_iterations ) + " iterations for the measuring." );

    log( "Cleanup." );
    for( int i = 0; i < n_sorted; ++i ) {
        if( not words[i].is_sorted() ) {
            return {};
        }
    }
    return m.average_seconds();
}

void cpp_main()
{
    cout << fixed;
    cout    << "Iterative \"natural runs\" merge-sort results in seconds, for "
            << english_words_list().count() << " words:"
            << endl;
    cout << endl;
    const auto w = setw( 16 );
    cout << w << "Sorted data:" << w << "Shuffled data:" << w << "Diff:" << endl;
    for( int i = 1; i <= 12; ++i ) {
        constexpr double nan = numeric_limits<double>::quiet_NaN();
        const auto& sorted_words    = *english_words_list;
        const auto& shuffled_words  = *[]{ return shuffled_english_words_list(); };

        const double sorted_time    = seconds_for( sorted_words ).value_or( nan );
        const double shuffled_time  = seconds_for( shuffled_words ).value_or( nan );
        cout    << w << sorted_time
                << w << shuffled_time
                << w << shuffled_time - sorted_time
                << endl;
    }
}

auto main()
    -> int
{
    try {
        cpp_main();
        return EXIT_SUCCESS;
    } catch( const exception& x ) {
        clog << "!" << x.what() << endl;
    }
    return EXIT_FAILURE;
}
