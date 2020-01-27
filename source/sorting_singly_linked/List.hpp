#pragma once
#include "../Type_.hpp"
#include "Node.hpp"

#include <stddef.h>         // ptrdiff_t
#include <algorithm>        // std::(exchange, swap)

namespace oneway_sorting_examples {
    using std::exchange, std::swap;
    using Size = ptrdiff_t;

    // Just taking charge of copying, moving and destruction, not encapsulating internals.
    // For convenience & DRYness provides a nested class `Appender` and a method `count`.
    struct List
    {
        Node* head;

        class Appender;                                         // Convenience.
        inline auto count() const -> Size;                      // Convenience.

        inline friend void swap( List& a, List& b ) noexcept;   // Declared for exposition.
        inline auto operator=( const List& other ) -> List&;    // Copy assignment.
        inline auto operator=( List&& other ) -> List&;         // Move assignment.
        inline List();                                          // Default constructor.
        inline List( const List& other );                       // Copy constructor.
        inline List( List&& other );                            // Move constructor.    
        inline ~List();                                         // Destructor.
    };

    class List::Appender
    {
        Node*&  m_head;
        Node*   m_last;

        Appender( const Appender& ) = delete;

    public:
        Appender( Node*& a_head_pointer ):
            m_head( a_head_pointer ),
            m_last( nullptr )
        {
            for( Node* p = m_head; p != nullptr; p = p->next ) {
                m_last = p;
            }
        }

        void append( const Type_<Node*> new_node )
        {
            Node*& beyond = (m_last == nullptr? m_head : m_last->next);
            new_node->link_in_before( beyond );
            m_last = new_node;
        }
    };

    inline auto List::count() const
        -> Size
    {
        Size n = 0;
        for( Node* p = head; p != nullptr; p = p->next ) {
            ++n;
        }
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
        
    inline auto List::operator=( List&& other )
        -> List&
    {
        head = exchange( other.head, nullptr );
        return *this;
    }

    inline List::List():
        head( nullptr )
    {}

    inline List::List( const List& other ):
        List()
    {
        Appender appender( head );
        for( Node* p = other.head; p != nullptr; p = p->next ) {
            appender.append( new Node{ nullptr, p->value } );
        }
    }
    
    inline List::List( List&& other ):
        head( exchange( other.head, nullptr ) )
    {}

    inline List::~List()
    {
        delete_list( head );
    }

}  // namespace oneway_sorting_examples
