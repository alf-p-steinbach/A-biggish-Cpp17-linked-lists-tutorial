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
    
    cout << n << " words:" << endl;
    int i = 0;
    for( Node* p = words.head; p != nullptr; p = p->next ) {
        if( i < 5 or n - 5 <= i ) {
            if( i > 0 ) {
                cout << ", ";
            }
            if( i == n - 5 ) {
                cout << "..., ";
            }
            cout << p-> value;
        }
        ++i;
    }
    cout << "." << endl;
}
