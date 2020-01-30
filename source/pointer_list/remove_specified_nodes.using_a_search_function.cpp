#include "list_copy_of_the_five_important_numbers.hpp"

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

template< class Func >
auto find( const Func& is_target, Node*& head )
    -> Node*&           // Reference to the next-field of the previous node.
{
    Node*   trailing    = nullptr;
    Node*   p           = head;
    for( ;; ) {
        if( p == nullptr or is_target( p->value ) ) {
            return (trailing == nullptr? head : trailing->next);
        }
        trailing = p;  p = p->next;
    }
    // The execution will never get here.
}

auto find_not_42( Node*& head )
    -> Node*&
{ return find( [](double x){ return x != 42; }, head ); }

auto main()
    -> int
{
    Node* head = list_copy_of_the_five_important_numbers();
    display( "Original values:", head );

    // Delete all nodes that are not 42, in a way that's O(n) efficient for a large list.
    cout << "O(n)-deleting the too math-ish numbers..." << endl;

    Node** p_sublist_head = &head;
    while( Node*& next_field = find_not_42( *p_sublist_head ) ) {
        delete unlinked( next_field );
        p_sublist_head = &next_field;
    }

    display( "The list is now", head );
    delete_list( +head );
}
