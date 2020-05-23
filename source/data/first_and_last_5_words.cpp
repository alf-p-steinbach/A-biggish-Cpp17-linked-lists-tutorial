#include "english_words_iteration.hpp"
#include "Abbreviated_list_displayer.hpp"
using data::Abbreviated_list_displayer, data::for_each_english_word, data::n_english_words;

#include <iostream>         // std::(cout, endl)
using std::cout, std::endl;

auto main()
    -> int
{
    cout << n_english_words << " words:" << endl;
    Abbreviated_list_displayer abbreviator( cout, n_english_words );
    for_each_english_word( [&](auto word) {
        abbreviator.display( word );
        } );
    cout << "." << endl;
}
