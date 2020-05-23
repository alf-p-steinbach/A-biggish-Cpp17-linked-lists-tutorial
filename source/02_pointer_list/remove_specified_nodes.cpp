#include "list_copy_of_the_five_important_numbers.hpp"
using util::Type_;

#include <iostream>
using std::cout, std::endl;

void display( const Type_<const char*> explanation, const Type_<Node*> head )
{
    cout << explanation;
    for( Node* p = head; p != nullptr; p = p->next ) {
        cout << " " << p->value;
    }
    cout << "." << endl;
}

auto main()
    -> int
{
    Node* head = list_copy_of_the_five_important_numbers();
    display( "Original values:", head );

    // Delete all nodes that are not 42, in a way that's O(n) efficient for a large list.
    cout << "O(n)-deleting the too math-ish numbers..." << endl;

    Node*   p           = head;
    Node*   trailing    = nullptr;
    while( p != nullptr ) {
        if( p->value != 42 ) {
            Node*& next_field = (trailing == nullptr? head : trailing->next);
            delete unlinked( next_field );
            p = next_field;
        } else {
            trailing = p;  p = p->next;
        }
    }

    display( "The list is now", head );
    delete_list_and_zero( head );
}
