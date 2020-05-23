#include "../my_chrono.hpp"
#include "../my_random.hpp"
using my_chrono::Timer_clock, my_chrono::Time_point, my_chrono::as_seconds;
using my_random::Seed;

#include "../data/Abbreviated_list_displayer.hpp"
using data::Abbreviated_list_displayer;

#include "english_words_list.hpp"
#include "merge_shuffle.hpp"
namespace x = techniques;
using std::string_view;
using x::Joinable_list_, x::english_words_list, x::merge_shuffle;

#include <iostream>
using std::fixed, std::cout, std::clog, std::endl;

template< class Func >
auto seconds_for( const Func& f )
    -> double
{
    const Time_point start_time = Timer_clock::now();
    f();
    const Time_point end_time = Timer_clock::now();
    return as_seconds( end_time - start_time );
}

auto main()
    -> int
{
    Joinable_list_<string_view> words = english_words_list();
    const int n = int( count_of( words ) );
    
    const double n_seconds = seconds_for( [&]
    {
        const int seq_nr = 42;
        merge_shuffle( words, Seed( seq_nr ) );     // seq_nr => same result every time.
    } );

    clog << fixed << n_seconds << " seconds." << endl;
    cout << "Merge-shuffled " << n << " words:" << endl;
    Abbreviated_list_displayer displayer( cout, n );
    words.for_each( [&]( const string_view& word )
    {
        displayer.display( word );
    } );
    cout << "." << endl;
}
