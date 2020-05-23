#include <forward_list>         // std::forward_list
#include <iostream>
using std::cout, std::endl, std::forward_list;

auto main()
    -> int
{
    const auto data = {3.14, 2.72, 0., 42., -1.};
    forward_list<double> list;

    cout << "Values:";
    for( const double v: data ) {
        cout << " " << v;
    }
    cout << "." << endl;

    // Prepend values to the list.
    for( const double v: data ) {
        list.push_front( v );
    }

    cout << "List contents after prepending the values:";
    for( const double v: list ) {
        cout << " " << v;
    }
    cout << "." << endl;
}
