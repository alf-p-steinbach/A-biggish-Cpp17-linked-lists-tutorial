#include "Node.hpp"

#include <initializer_list>     // Required for deduced type of brace initializer.
#include <iostream>
using std::cout, std::endl;

auto find_position( const double new_value, Node*& list_head )
    -> Node*&
{
    Node* trailing = nullptr;
    for(    Node* p = list_head; 
            p != nullptr and p->value < new_value;
            p = p->next ) {
        trailing = p;
    }
    return (trailing == nullptr? list_head : trailing->next);
}

auto main()
    -> int
{
    const auto data = {3.14, 2.72, 0., 42., -1.};
    Node* head = nullptr;

    // Insert the data sorted.
    for( const double v: data ) {
        Node*& next = find_position( v, head );
        (new Node{ nullptr, v})->link_in_before( next );
    }

    // Display the list.
    for( Node*p = head; p != nullptr; p = p->next ) {
        cout << p->value << " ";
    }
    cout << endl;

    // Clean up, just to be nice.
    delete_list( +head );
}
