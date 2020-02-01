#include "../my_chrono.hpp"
using my_chrono::Timer_clock, my_chrono::Time_point, my_chrono::as_seconds;

#include "../data/Abbreviated_list_displayer.hpp"
using data::Abbreviated_list_displayer;

#include "shuffled_english_words_list.hpp"
#include "merge_sort_iteratively.hpp"
namespace x = oneway_sorting_examples;
using x::Node, x::List, x::shuffled_english_words_list, x::merge_sort_iteratively;

#include <iostream>
using std::fixed, std::cout, std::clog, std::endl;

auto main()
    -> int
{
    List words = shuffled_english_words_list();

    if(0)
    {
        Node* p = words.head;
        for( int i = 1; i <= 12; ++i ) { p = p->next; }
        delete_list_and_zero( p->next );
    }

    const int n = int( words.count() );
    clog << n << endl;
    
    const Time_point start_time = Timer_clock::now();
    const int n_passes = merge_sort_iteratively( words );
    const Time_point end_time = Timer_clock::now();
    const double n_seconds = as_seconds( end_time - start_time );

    clog << fixed << n_seconds << " seconds, using " << n_passes << " passes." << endl;
    cout << "Iteratively merge-sorted " << n << " words:" << endl;
    Abbreviated_list_displayer displayer( cout, n );
    for( Node* p = words.head; p != nullptr; p = p->next ) {
        displayer.display( p->value );
    }
    cout << "." << endl;

    if( 0 ) {
        std::string_view previous;
        for( Node* p = words.head; p != nullptr; p = p->next ) {
            if( p->value < previous ) {
                cout << "|";
            }
            cout << p->value << " ";
            previous = p->value;
        }
        cout << endl;
    }
}
