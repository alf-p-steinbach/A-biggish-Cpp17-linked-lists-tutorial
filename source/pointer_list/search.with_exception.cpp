#include "list_copy_of_the_five_important_numbers.hpp"

#include <math.h>           // ::abs
#include <stdlib.h>         // EXIT_...
    
#include <iostream>
#include <stdexcept>        // std::(runtime_error, exception)

namespace app {
    using std::cout, std::endl, std::runtime_error;
    using namespace std::literals;      // ""s

    void display( const Type_<const char*> explanation, const Type_<Node*> head )
    {
        cout << explanation;
        for( Node* p = head; p != nullptr; p = p->next ) {
            cout << " " << p->value;
        }
        cout << "." << endl;
    }

    struct Not_found: runtime_error { using runtime_error::runtime_error; };

    template< class Func >
    auto next_field_pointing_to_node( const Func& has_desired_property, Node*& head )
        -> Node*&           // Reference to next-field
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
        throw Not_found( ""s + __func__ + " - failed to find specified node." );
    }

    void run()
    {
        struct List     // If you have such class derive this from a `No_copy_or_move`.
        {
            Node* head = list_copy_of_the_five_important_numbers();
            ~List() { delete_list( head ); }        // Auto cleanup also when exception.
        };
        
        List list;

        display( "Original values:", list.head );
    #if defined( FAIL_PLEASE )

        // If there is no node with value 7 then this just throws, which is OK.
        const auto with_value_7 = [](double x) -> bool { return x == 7; };
        delete unlinked( next_field_pointing_to_node( with_value_7, list.head ) );

    #elif defined( EFFICIENT_PLEASE )

        // Delete all nodes that are not 42, in a way that's O(n) efficient for a large list.
        cout << "O(n)-deleting the too math-ish numbers..." << endl;
        try {
            const auto not_42 = [](double x) -> bool { return x != 42; };
            Node** pp_sublist_head = &list.head;
            for( ;; ) {
                Node*& next = next_field_pointing_to_node( not_42, *pp_sublist_head );
                delete unlinked( next );
                pp_sublist_head = &next;    // Search only in the part of the list after this.
            }
        } catch( const Not_found& ) {
            ;   // Ignore the exception, because it just means that no node was found.
        }

    #else

        // Delete all nodes that are not 42, in a simple but O(n^2) way.
        cout << "O(n^2)-deleting the too math-ish numbers..." << endl;
        try {
            const auto not_42 = [](double x) -> bool { return x != 42; };
            for( ;; ) {
                delete unlinked( next_field_pointing_to_node( not_42, list.head ) );
            }
        } catch( const Not_found& ) {
            ;   // Ignore the exception, because it just means that no node was found.
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
