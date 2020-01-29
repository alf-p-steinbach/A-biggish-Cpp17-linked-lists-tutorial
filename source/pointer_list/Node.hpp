#pragma once
#include "../Type_.hpp"

struct Node
{
    Node*   next;
    double  value;

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
    
    friend void delete_list( Node* head ) noexcept
    {
        while( head ) { delete unlinked( head ); }
    }
};
