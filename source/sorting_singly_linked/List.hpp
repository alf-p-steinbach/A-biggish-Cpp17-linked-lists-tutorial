#pragma once
#include "../signed_size_types.hpp"
#include "../Type_.hpp"
#include "Node.hpp"

#include <algorithm>        // std::(exchange, swap)
#include <utility>          // std::move

namespace oneway_sorting_examples {
    using std::exchange, std::move, std::swap;
    using signed_size_types::Size, signed_size_types::Index;

    // Just taking charge of copying, moving and destruction, not encapsulating internals.
    // For convenience & DRYness provides a nested class `Appender` and a method `count`.
    struct List
    {
        Node* head;

        class Appender;                                         // Convenience.
        inline auto count() const noexcept -> Size;             // Convenience.

        inline friend void swap( List& a, List& b ) noexcept;   // Declared for exposition.
        inline auto operator=( const List& other ) -> List&;    // Copy assignment.
        inline auto operator=( List&& other ) noexcept -> List&;// Move assignment.
        inline List() noexcept;                                 // Default constructor.
        inline List( const List& other );                       // Copy constructor.
        inline List( List&& other ) noexcept;                   // Move constructor.    
        inline ~List() noexcept;                                // Destructor.
    };

    class List::Appender
    {
        Node*&  m_head;
        Node*   m_last;

        Appender( const Appender& ) = delete;

    public:
        Appender( Node*& a_head_pointer ) noexcept:
            m_head( a_head_pointer ),
            m_last( nullptr )
        {
            for( Node* p = m_head; p; p = p->next ) { m_last = p; }
        }

        void append( const Type_<Node*> new_node ) noexcept
        {
            Node*& beyond = (m_last? m_last->next : m_head);
            new_node->link_in_before( beyond );
            m_last = new_node;
        }
    };

    inline auto List::count() const noexcept
        -> Size
    {
        Size n = 0;
        for( Node* p = head; p; p = p->next ) { ++n; }
        return n;
    }

    inline void swap( List& a, List& b ) noexcept
    {
        swap( a.head, b.head );
    }

    inline auto List::operator=( const List& other )
        -> List&
    {
        List temp = other;
        swap( temp, *this );
        return *this;
    }
        
    inline auto List::operator=( List&& other ) noexcept
        -> List&
    {
        // Can be optimized, but delegation to constructor + swap is nicely DRY.
        List temp = move( other );
        swap( temp, *this );
        return *this;
    }

    inline List::List() noexcept:
        head( nullptr )
    {}

    inline List::List( const List& other ):
        List()
    {
        Appender appender( head );
        try {
            for( Node* p = other.head; p; p = p->next ) {
                appender.append( new Node{ nullptr, p->value } );
            }
        } catch( ... ) {
            delete_list( +head );
            throw;
        }
    }
    
    inline List::List( List&& other ) noexcept:
        head( exchange( other.head, nullptr ) )
    {}

    inline List::~List() noexcept
    {
        delete_list( +head );
    }

}  // namespace oneway_sorting_examples
