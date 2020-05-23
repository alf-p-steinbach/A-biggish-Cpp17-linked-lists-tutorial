#pragma once
#include "../util/Type_.hpp"
#include "../util/value.hpp"    // Workaround for MSVC 2019: express `+p` as `value(p)`.

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
        using util::Type_;

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
