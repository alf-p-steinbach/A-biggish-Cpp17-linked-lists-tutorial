﻿#include "../data/Abbreviated_list_displayer.hpp"
using data::Abbreviated_list_displayer;

#include "shuffled_english_words_list.hpp"
namespace x = oneway_sorting_examples;
using x::Node, x::List, x::shuffled_english_words_list;

#include <iostream>
using std::cout, std::endl;

auto main()
    -> int
{
    List words = shuffled_english_words_list();
    const int n = int( words.count() );
    
    cout << "`shuffled_english_words_list()` produced " << n << " words:" << endl;
    
    Abbreviated_list_displayer displayer( cout, n );
    for( Node* p = words.head; p != nullptr; p = p->next ) {
        displayer.display( p->value );
    }
    cout << "." << endl;
}
