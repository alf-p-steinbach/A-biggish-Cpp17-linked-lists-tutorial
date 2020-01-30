#pragma once
#include "../Type_.hpp"
#include <string_view>

namespace oneway_sorting_examples {
    using std::string_view;

    struct Node
    {
        Node*           next;
        string_view     value;

        void link_in_before( Node*& a_next_field ) noexcept
        {
            next = a_next_field;
            a_next_field = this;
        }

        friend auto unlinked( Node*& a_next_field ) noexcept
            -> Node*
        {
            const Type_<Node*> result = a_next_field;
            a_next_field = result->next;
            return result;
        }
        
        friend void delete_list_and_zero( Node*& head ) noexcept
        {
            while( head ) { delete unlinked( head ); }
        }

        friend void delete_list( Node*&& temp ) noexcept
        {
            delete_list_and_zero( temp );
        }
    };
}  // namespace oneway_sorting_examples
