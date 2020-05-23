#include "list_copy_of_the_five_important_numbers.hpp"

#include <math.h>           // ::abs
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
auto next_field_pointing_to_node( const Func& has_desired_property, Node*& head )
    -> Node**           // Pointer to next-field
{
    Node* trailing = nullptr;
    for(    Node* p = head;
            p != nullptr;
            p = p->next ) {
        if( has_desired_property( p->value ) ) {
            return (trailing == nullptr? &head : &trailing->next);
        }
        trailing = p;
    }
    return nullptr;
}

auto main()
    -> int
{
    Node* head = list_copy_of_the_five_important_numbers();

    display( "Original values:", head );
#if defined( UB_PLEASE )

    // The pointer to next field function result makes it easy to do this inadvertently...
    const auto with_value_7 = [](double x) -> bool { return x == 7; };
    delete unlinked( *next_field_pointing_to_node( with_value_7, head ) );

#elif defined( EFFICIENT_PLEASE )

    // Delete all nodes that are not 42, in a way that's O(n) efficient for a large list.
    const auto not_42 = [](double x) -> bool { return x != 42; };
    Node** pp_sublist_head = &head;
    while( Node** pp_doomed = next_field_pointing_to_node( not_42, *pp_sublist_head ) ) {
        delete unlinked( *pp_doomed );
        pp_sublist_head = pp_doomed;    // Search only in the part of the list after this.
    }

#else

    // Delete all nodes that are not 42, in a simple but O(n^2) way.
    const auto with_not_42 = [](double x) -> bool { return x != 42; };
    while( Node** pp_doomed = next_field_pointing_to_node( with_not_42, head ) ) {
        delete unlinked( *pp_doomed );
    }

#endif
    display( "After deleting the too math-ish numbers the list is", head );

    delete_list( +head );
}
