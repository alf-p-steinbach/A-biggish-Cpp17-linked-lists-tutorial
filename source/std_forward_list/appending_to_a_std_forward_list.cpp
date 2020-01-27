#include <forward_list>         // std::forward_list
#include <iostream>
using std::cout, std::endl, std::forward_list;

auto main()
    -> int
{
    forward_list<double> values;

    { // Append values to the list.
        auto it = values.before_begin();
        for( const double v: {3.14, 2.72, 0., 42., -1.} ) {
            it = values.insert_after( it, v );
        }
    }

    for( const double v: values ) {
        cout << v << " ";
    }
    cout << endl;
}
