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

template< class Func >
auto list_of_all( const Func& is_target, Node*& head )
    -> Node*
{
    // Transfer all nodes that fit, in a way that's O(n) efficient for a large list.
    Node*   transferred_head    = nullptr;
    Node**  p_sublist_head      = &head;
    while( Node*& next_field = find( is_target, *p_sublist_head ) ) {
        unlinked( next_field )->link_in_before( transferred_head );
        p_sublist_head = &next_field;
    }
    return transferred_head;
}

auto list_of_all_not_42_in( Node*& head )
    -> Node*
{ return list_of_all( [](double x){ return x != 42; }, head ); }

auto main()
    -> int
{
    Node* head = list_copy_of_the_five_important_numbers();
    display( "Original values:", head );

    cout << "O(n)-deleting the too math-ish numbers..." << endl;
    delete_list( list_of_all_not_42_in( head ) );

    display( "The list is now", head );
    delete_list( +head );
}
