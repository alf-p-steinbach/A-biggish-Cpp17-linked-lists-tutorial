#pragma once
#include "../Type_.hpp"

struct Node
{
    Node*   next;
    double  value;

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
