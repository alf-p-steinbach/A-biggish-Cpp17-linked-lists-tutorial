#include "../my_chrono.hpp"
using my_chrono::Timer_clock, my_chrono::Time_point, my_chrono::as_seconds;

#include "../data/Abbreviated_list_displayer.hpp"
using data::Abbreviated_list_displayer;

#include "english_words_list.hpp"
#include "merge_sort_recursively.hpp"
namespace x = oneway_sorting_examples;
using x::Node, x::List, x::english_words_list, x::merge_sort_recursively;

#include <iostream>
using std::fixed, std::cout, std::clog, std::endl;

auto main()
    -> int
{
    List words = english_words_list();
    const int n = int( words.count() );
    
    const Time_point start_time = Timer_clock::now();
    merge_sort_recursively( words );
    const Time_point end_time = Timer_clock::now();
    const double n_seconds = as_seconds( end_time - start_time );

    clog << fixed << n_seconds << " seconds." << endl;
    cout << "Recursively merge-sorted " << n << " words:" << endl;
    Abbreviated_list_displayer displayer( cout, n );
    for( Node* p = words.head; p != nullptr; p = p->next ) {
        displayer.display( p->value );
    }
    cout << "." << endl;
}
