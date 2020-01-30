#include "list_copy_of_the_five_important_numbers.hpp"

#include <stdlib.h>         // EXIT_...
    
#include <functional>       // std::(ref, reference_wrapper)
#include <iostream>
#include <optional>         // std::optional
#include <stdexcept>        // std::exception

// throw Not_found( ""s + __func__ + " - failed to find specified node." );

namespace app {
    using std::cout, std::endl, std::optional, std::reference_wrapper;

    void display( const Type_<const char*> explanation, const Type_<Node*> head )
    {
        cout << explanation;
        for( Node* p = head; p != nullptr; p = p->next ) {
            cout << " " << p->value;
        }
        cout << "." << endl;
    }

    using Pos = optional<reference_wrapper<Node*>>;     // Ref. to next-field, or empty.
    auto ref_of( Pos pos ) -> Node*& { return pos.value().get(); }

    template< class Func >
    auto next_field_pointing_to_node( const Func& has_desired_property, Node*& head )
        -> Pos
    {
        Node* trailing = nullptr;
        for(    Node* p = head;
                p != nullptr;
                p = p->next ) {
            if( has_desired_property( p->value ) ) {
                return (trailing == nullptr? head : trailing->next);
            }
            trailing = p;
        }
        return {};
    }

    void run()
    {
        struct List     // If you have such class derive this from a `No_copy_or_move`.
        {
            Node* head = list_copy_of_the_five_important_numbers();
            ~List() { delete_list( +head ); }       // Auto cleanup also when exception.
        };
        
        List list;

        display( "Original values:", list.head );
    #if defined( FAIL_PLEASE )

        // If there is no node with value 7 then the implicit conversion from wrapper to
        // `Node*&` just throws, which is OK, but the exception message is not informative.

        const auto with_value_7 = [](double x) -> bool { return x == 7; };
        delete unlinked( ref_of( next_field_pointing_to_node( with_value_7, list.head ) ) );

    #elif defined( EFFICIENT_PLEASE )

        // Delete all nodes that are not 42, in a way that's O(n) efficient for a large list.
        // The `while` condition uses the wrapper's implicit conversion to `bool`.

        cout << "O(n)-deleting the too math-ish numbers..." << endl;
        const auto not_42 = [](double x) -> bool { return x != 42; };
        Node** pp_sublist_head = &list.head;
        while( Pos next = next_field_pointing_to_node( not_42, *pp_sublist_head ) ) {
            delete unlinked( ref_of( next ) );
            pp_sublist_head = &ref_of( next );  // Search only in the rest of the list.
        }

    #else

        // Delete all nodes that are not 42, in a simple but O(n^2) way.

        cout << "O(n^2)-deleting the too math-ish numbers..." << endl;
        const auto not_42 = [](double x) -> bool { return x != 42; };
        while( Pos next = next_field_pointing_to_node( not_42, list.head ) ) {
            delete unlinked( ref_of( next ) );
        }

    #endif
        display( "The list is now", list.head );
    }
}  // namespace app

auto main()
    -> int
{
    using std::exception, std::cerr, std::endl;
    try {
        app::run();
        return EXIT_SUCCESS;
    } catch( const exception& x ) {
        cerr << "!" << x.what() << endl;
    }
    return EXIT_FAILURE;
}
