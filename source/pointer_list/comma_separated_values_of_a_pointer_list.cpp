#include "list_copy_of_the_five_important_numbers.hpp"

#include <iostream>
using std::cout, std::endl;

auto main()
    -> int
{
    const Type_<Node*> head = list_copy_of_the_five_important_numbers();

    for( Node* p = head; p != nullptr; p = p->next ) {
        if( p != head ) {
            // Not at the first node, so add text to separate the previous value.
            const bool is_at_last_node = (p->next == nullptr);
            cout << (is_at_last_node? " and " : ", ");
        }
        cout << p->value;
    }
    cout << "." << endl;

    delete_list( +head );
}
