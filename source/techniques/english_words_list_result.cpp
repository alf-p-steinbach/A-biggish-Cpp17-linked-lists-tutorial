#include "../data/Abbreviated_list_displayer.hpp"
using Displayer = data::Abbreviated_list_displayer;

#include "english_words_list.hpp"
using std::string_view;
using techniques::english_words_list, techniques::Joinable_list_;

#include <iostream>         // std::(cout, endl)
using std::cout, std::endl;

auto main()
    -> int
{
    const Joinable_list_<string_view> words = english_words_list();
    const int n = int( count_of( words ) );
    
    cout << "`english_words_list_()` produced " << n << " words:" << endl;
    Displayer displayer( cout, n );
    words.for_each( [&]( const string_view& word ){ displayer.display( word ); } );
    cout << "." << endl;
}
