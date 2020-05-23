#include "adaptive_timing_block_allocator.hpp"

#include "../util/std_chrono.hpp"
#include "../util/std_random.hpp"
using namespace util::std_chrono::_alias;
using namespace util::std_random::_alias;
using std_chrono::Measurement, std_chrono::time_per;

#include "shuffled_english_words_list.hpp"
#include "merge_sort_iteratively.hpp"
namespace x = oneway_sorting_examples;
using
    x::english_words_list, x::shuffled_english_words_list,
    x::Node, x::List, x::merge_sort_iteratively;

#include <iomanip>          // std::setw
#include <iostream>         // std::(fixed, cout, clog, endl)    
#include <limits>           // std::numeric_limits
#include <optional>         // std::optional
#include <stdexcept>        // std::(exception, runtime_error)
#include <string>           // std::string
#include <vector>           // std::vector
using
    std::exception, std::runtime_error, std::string, std::to_string, std::vector,
    std::numeric_limits, std::optional,
    std::setw, std::fixed, std::cout, std::clog, std::endl;

void log( const string& s )
{
    // Turn off logging output by redirecting the error stream, e.g. in Windows `a 2>nul`.
    clog << "- " << s << endl;
}

constexpr double nan_value = numeric_limits<double>::quiet_NaN();

auto seconds_for( const List& words_list )
    -> double
{
    log( "Preparing data." );
    vector<List> words( 2048, words_list );
    const int n_lists = static_cast<int>( words.size() );
    int n_sorted = 0;

    log( "Measuring" );
    const Measurement m = time_per( [&]
    {
        if( n_sorted == n_lists ) { throw runtime_error( "Too few prepared lists." ); }
        merge_sort_iteratively( words[n_sorted] );
        ++n_sorted;
    } );
    log( "Used " + to_string( m.n_iterations ) + " iterations for the measuring." );

    log( "Cleanup." );
    for( int i = 0; i < n_sorted; ++i ) {
        if( not words[i].is_sorted() ) {
            return nan_value;
        }
    }
    return m.average_seconds();
}

auto count_the_words()
    -> int
{
    int n = 0;
    data::for_each_english_word( [&](auto){ ++n; } );
    return n;
}

void cpp_main()
{
    log( "Starting." );
    const int n_words = count_the_words();      // Avoids dynamic allocations, to be nice.
    #ifdef USE_CUSTOM_NODE_ALLOCATION
        log( "Using custom node allocation to optimize things." );
        x::Block_allocator::instance().reserve( (1 + 2048)*n_words );
    #endif

    cout << fixed;
    cout    << "Iterative \"natural runs\" merge-sort results in seconds, for "
            << n_words << " words:"
            << endl;
    cout << endl;
    const auto w = setw( 16 );
    cout << w << "Sorted data:" << w << "Shuffled data:" << w << "Diff:" << endl;
    for( int i = 1; i <= 12; ++i ) {
        const double sorted_time    = seconds_for( english_words_list() );
        const double shuffled_time  = seconds_for( shuffled_english_words_list() );
        const double diff           = shuffled_time - sorted_time;
        cout << w << sorted_time << w << shuffled_time << w << diff << endl;
    }

    log( "Finished." );
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
