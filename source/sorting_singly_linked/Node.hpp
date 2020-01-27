#pragma once
#include <string_view>

namespace oneway_sorting_examples {
    using std::string_view;

    struct Node
    {
        Node*           next;
        string_view     value;

        void link_in_before( Node*& a_next_field )
        {
            next = a_next_field;
            a_next_field = this;
        }
        
        friend auto unlinked( Node*& a_next_field )
            -> Node*
        {
            const Type_<Node*> result = a_next_field;
            a_next_field = result->next;
            return result;
        }
        
        friend void delete_list( Node* head )
        {
            while( head != nullptr ) {
                delete unlinked( head );
            }
        }
    };
}  // namespace oneway_sorting_examples
