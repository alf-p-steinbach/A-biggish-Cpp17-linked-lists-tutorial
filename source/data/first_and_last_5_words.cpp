#include "english_words_iteration.hpp"
#include "Abbreviated_list_displayer.hpp"
using data::Abbreviated_list_displayer, data::for_each_english_word;

#include <iostream>         // std::(cout, endl)
#include <functional>       // std::invoke
using std::cout, std::endl, std::invoke;

auto main()
    -> int
{
    const int n = invoke( []() -> int
    {
        int count = 0;
        for_each_english_word( [&](auto) { ++count; } );
        return count;
    } );
    
    cout << n << " words:" << endl;
    Abbreviated_list_displayer displayer( cout, n );
    for_each_english_word( [&](auto word){ displayer.display( word ); } );
    cout << "." << endl;
}
