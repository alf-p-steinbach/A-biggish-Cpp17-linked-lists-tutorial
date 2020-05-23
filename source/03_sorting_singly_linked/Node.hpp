#pragma once
#include "../util/signed_size_types.hpp"
#include "../util/Type_.hpp"

#include <string_view>

namespace oneway_sorting_examples {
    using std::string_view;
    using util::Size, util::Type_;

    struct Node final
    {
        Node*           next;
        string_view     value;

        inline void link_in_before( Node*& a_next_field ) noexcept;

        friend inline auto unlinked( Node*& a_next_field ) noexcept -> Node*;
        friend inline void delete_list_and_zero( Node*& head ) noexcept;
        friend inline void delete_list( Node*&& temp ) noexcept;

        #ifdef USE_CUSTOM_NODE_ALLOCATION
            static inline auto operator new( const size_t size ) -> void*;
            static inline void operator delete( const Type_<void*> p );
        #endif
    };
    
    inline void Node::link_in_before( Node*& a_next_field ) noexcept
    {
        next = a_next_field;
        a_next_field = this;
    }

    inline auto unlinked( Node*& a_next_field ) noexcept
        -> Node*
    {
        const Type_<Node*> result = a_next_field;
        a_next_field = result->next;
        return result;
    }
    
    inline void delete_list_and_zero( Node*& head ) noexcept
    {
        while( head ) { delete unlinked( head ); }
    }

    inline void delete_list( Node*&& temp ) noexcept
    {
        delete_list_and_zero( temp );
    }

    // Support for the custom node allocation machinery:

    class Node_allocator
    {
        Node_allocator( const Node_allocator& ) = delete;

    protected:
        Node_allocator() {}

    public:
        virtual auto allocate( Size ) -> void* = 0;
        virtual void deallocate( void* ) = 0;

        static auto instance() -> Node_allocator&;      // Defined by client code.
    };

    #ifdef USE_CUSTOM_NODE_ALLOCATION
        inline auto Node::operator new( const size_t size )
            -> void*
        { return Node_allocator::instance().allocate( size ); }

        inline void Node::operator delete( const Type_<void*> p )
        {
            Node_allocator::instance().deallocate( p );
        }
    #endif

}  // namespace oneway_sorting_examples
