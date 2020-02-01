#include "../my_chrono.hpp"
#include "../my_random.hpp"
using my_chrono::Timer_clock, my_chrono::Time_point, my_chrono::as_seconds;

#include "../data/Abbreviated_list_displayer.hpp"
using data::Abbreviated_list_displayer;

#include "shuffled_english_words_list.hpp"
#include "merge_sort_recursively.hpp"
namespace x = oneway_sorting_examples;
using
    x::english_words_list, x::shuffled_english_words_list,
    x::Node, x::List, x::merge_sort_recursively;
using Words_list_func = auto()->List;

#include <iostream>
using std::vector, std::string_view, std::fixed, std::cout, std::clog, std::endl;

void test( Words_list_func& words_list, Type_<const char*> list_description )
{
    List words = words_list();
    const int n = int( words.count() );
    
    const Time_point    start_time  = Timer_clock::now();
    merge_sort_recursively( words );
    const Time_point    end_time    = Timer_clock::now();
    const double        n_seconds   = as_seconds( end_time - start_time );
    const bool          ok          = words.is_sorted();

    clog << fixed << n_seconds << " seconds." << endl;
    cout    << "Recursively merge-sorted " << n << " words"
            << " from " << list_description << "()"
            << " with " << (ok? "sorted" : "UNGOOD") << " result:"
            << endl;
    Abbreviated_list_displayer displayer( cout, n );
    for( const Node* p = words.head; p != nullptr; p = p->next ) {
        displayer.display( p->value );
    }
    cout << "." << endl;
}

#define TEST( f ) test( *[]{return f();}, #f )

auto main()
    -> int
{
    using my_random::Seed;
    clog << endl;
    TEST( english_words_list );
    TEST( shuffled_english_words_list );
}
