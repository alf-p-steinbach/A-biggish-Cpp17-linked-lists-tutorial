#pragma once
#include "../Type_.hpp"

#ifdef USE_FREE_LIST
#   include "Free_list_.hpp"
#endif

namespace techniques{

    template< class Value >
    struct Node_ final
    {
        #ifdef USE_FREE_LIST
            static auto operator new( const size_t size )
                -> void*
            { return Free_list_<Node_>::instance().allocate( size ); }

            static void operator delete( const Type_<void*> p )
            {
                Free_list_<Node_>::instance().deallocate( p );
            }
        #endif

        Node_*      next;
        Value       value;

        void link_in_before( Node_*& a_next_field ) noexcept
        {
            next = a_next_field;
            a_next_field = this;
        }

        friend auto unlinked( Node_*& a_next_field ) noexcept
            -> Node_*
        {
            const Type_<Node_*> result = a_next_field;
            a_next_field = result->next;
            return result;
        }
        
        friend void delete_list_and_zero( Node_*& head ) noexcept
        {
            while( head ) { delete unlinked( head ); }
        }

        friend void delete_list( Node_*&& temp ) noexcept
        {
            delete_list_and_zero( temp );
        }
    };

}  // namespace techniques
