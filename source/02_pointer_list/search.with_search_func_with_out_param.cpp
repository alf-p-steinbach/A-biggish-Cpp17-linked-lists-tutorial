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
[[nodiscard]] // Better check the result, or cast to void to suppress a discard warning.
auto find_next_field_pointing_to_node (
    const Func&     has_desired_property,
    Node*&          head,                   // Start of (sub-) list to search.
    Node**&         result                  // Out-param, a pointer to a `next`-field.
    ) -> bool
{
    Node* trailing = nullptr;
    for(    Node* p = head;
            p != nullptr;
            p = p->next ) {
        if( has_desired_property( p->value ) ) {
            result = (trailing == nullptr? &head : &trailing->next);
            return true;
        }
        trailing = p;
    }
    return false;
}

auto main()
    -> int
{
    Node* head = list_copy_of_the_five_important_numbers();

    display( "Original values:", head );
#if defined( UB_PLEASE )

    // It's unnatural to do the cast to `void` without meaning to, so it becomes unlikely.
    Node** pp_doomed;                   // Indeterminate value after the declaration.
    const auto with_value_7 = [](double x) -> bool { return x == 7; };
    (void) find_next_field_pointing_to_node( with_value_7, head, pp_doomed );
    delete unlinked( *pp_doomed );      // If `pp_doomed` is still indeterminate then UB.

#elif defined( EFFICIENT_PLEASE )

    // Delete all nodes that are not 42, in a way that's O(n) efficient for a large list.
    const auto with_not_42 = [](double x) -> bool { return x != 42; };
    Node** pp_sublist_head = &head;
    Node** pp_doomed;
    while( find_next_field_pointing_to_node( with_not_42, *pp_sublist_head, pp_doomed ) ) {
        delete unlinked( *pp_doomed );
        pp_sublist_head = pp_doomed;    // Search only in the part of the list after this.
    }

#else

    // Delete all nodes that are not 42, in a simple but O(n^2) way.
    const auto with_not_42 = [](double x) -> bool { return x != 42; };
    Node** pp_doomed;
    while( find_next_field_pointing_to_node( with_not_42, head, pp_doomed ) ) {
        delete unlinked( *pp_doomed );
    }

#endif
    display( "After deleting the too math-ish numbers the list is", head );

    delete_list( +head );
}
