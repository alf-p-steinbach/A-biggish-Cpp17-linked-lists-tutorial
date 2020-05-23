#include "../data/english_words_iteration.hpp"
#include "../data/Abbreviated_list_displayer.hpp"
using   data::for_each_english_word, data::Abbreviated_list_displayer;

#include "../util/timing.hpp"
#include "../util/std_random.hpp"
using   util::timing::Timer;
using   namespace util::std_random::_alias;

#include <stddef.h>         // ptrdiff_t
using   Size = ptrdiff_t;
using   Index = Size;

#include <iostream>         // std::(cout, endl)
#include <vector>           // std::vector
#include <string_view>      // std::string_view
#include <utility>          // std::swap
using   std::fixed, std::cout, std::clog, std::endl,
        std::vector, std::string_view, std::swap;

auto english_words_vector()
    -> vector<string_view>
{
    vector<string_view> words;
    for_each_english_word( [&]( const string_view& w ) {
        words.push_back( w );
        } );
    return words;
}

void array_shuffle( vector<string_view>& words )
{
    using Random_index = std_random::Integer_<Index>;

    std_random::Bits bits( std_random::Seed( 42 ) );
    const Size max_index = words.size() - 1;
    for( Index i = 0; i < max_index; ++i ) {
        const Size  n_available     = words.size() - i;
        const Index offset_other    = Random_index::from( bits, n_available );
        if( offset_other != 0 ) {
            swap( words[i], words[i + offset_other] );
        }
    }
}

auto main()
    -> int
{
    vector<string_view> words = english_words_vector();
    const int n = int( words.size() );
    
    Timer timer;
    array_shuffle( words );
    const double n_seconds = timer.elapsed_seconds();

    clog << fixed << n_seconds << " seconds per shuffle." << endl;
    cout << "Array-shuffled " << n << " words:" << endl;
    Abbreviated_list_displayer displayer( cout, n );
    for( const string_view& word: words ) { displayer.display( word ); }
    cout << "." << endl;
}
