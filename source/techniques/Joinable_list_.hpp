#pragma once
#include "../signed_size_types.hpp"
#include "Movable.hpp"
#include "Node_.hpp"

#include <assert.h>         // assert

#include <stdexcept>        // std::runtime_error
#include <type_traits>      // std::is_same_v
#include <utility>          // std::(exchange, move, swap)

namespace techniques {
    using std::exchange, std::is_same_v, std::move;
    using signed_size_types::Size;
    
    template< class Value_type >
    class Joinable_list_
        : public Movable        // Movable but not copyable.
    {
    public:
        using Value     = Value_type;
        using Node      = Node_<Value>;

    private:
        Node*       m_head;
        Node*       m_last;

        auto popped_front() noexcept
            -> Node*
        {
            const Type_<Node*> result = unlinked( m_head );
            if( not m_head ) {
                m_last = nullptr;
            }
            return result;      // Note: the node's next-field is not necessarily null.
        }

    public:
        auto is_empty() const noexcept
            -> bool
        { return not m_head; }

        auto is_empty_or_single_value() const noexcept
            -> bool
        { return not m_head or not m_head->next; }

        auto front() const noexcept
            -> const Value&
        { assert( not is_empty() ); return m_head->value; }
        
        auto back() const noexcept
            -> const Value&
        { assert( not is_empty() ); return m_last->value; }

        void push_front( Value v )
        {
            m_head = new Node{ m_head, move( v ) };
            if( not m_last ) {
                m_last = m_head;
            }
        }
        
        void push_back( Value v )
        {
            if( is_empty() ) {
                push_front( move( v ) );
            } else {
                (new Node{ nullptr, move( v ) })->link_in_before( m_last->next );
                m_last = m_last->next;
            }
        }

        void pop_front() noexcept
        {
            assert( not is_empty() );
            delete popped_front();
        }

        void pop_back() noexcept = delete;      // Would be linear time.

        auto detached_front() noexcept
            -> Joinable_list_
        {
            assert( not is_empty() );

            Joinable_list_ result;
            result.m_head = popped_front();
            result.m_head->next = nullptr;
            result.m_last = result.m_head;
            return result;
        }

        void append( Joinable_list_&& other ) noexcept
        {
            Node*& beyond = (is_empty()? m_head : m_last->next);
            beyond = exchange( other.m_head, nullptr );
            if( other.m_last ) {
                m_last = exchange( other.m_last, nullptr );
            }
        }

        template< class Func >
        void for_each( const Func& f ) const
        {
            for( const Node* p = m_head; p; p = p->next ) {
                using R = decltype( f( p->value ) );
                if constexpr( is_same_v<R, bool> ) {
                    if( not f( p->value ) ) {
                        break;
                    }
                } else {
                    f( p->value );
                }
            }
        }

        void swap( Joinable_list_& a, Joinable_list_& b ) noexcept
        {
            using std::swap;
            swap( a.m_head, b.m_head );
            swap( a.m_last, b.m_last );
        }

        auto operator=( Joinable_list_&& other ) noexcept
            -> Joinable_list_&
        {
            Joinable_list_ temp( move( other ) );
            swap( temp, *this );
            return *this;
        }

        Joinable_list_() noexcept:
            m_head( nullptr ),
            m_last( nullptr )
        {}
        
        Joinable_list_( Joinable_list_&& other ) noexcept:
            m_head( exchange( other.m_head, nullptr ) ),
            m_last( exchange( other.m_last, nullptr ) )
        {}
        
        ~Joinable_list_() noexcept
        {
            while( m_head ) { delete unlinked( m_head ); }
        }
    };

    template< class Value >
    inline auto joined( Joinable_list_<Value>&& a, Joinable_list_<Value>&& b )
        -> Joinable_list_<Value>
    {
        a.append( move( b ) );
        return move( a );
    }

    template< class Value >
    inline auto count_of( const Joinable_list_<Value>& list )
        -> Size
    {
        Size size = 0;
        list.for_each( [&]( const Value& ) { ++size; } );
        return size;
    }

    template< class Value >
    inline auto copy_of( const Joinable_list_<Value>& list )
        -> Joinable_list_<Value>
    {
        Joinable_list_<Value> result;
        list.for_each( [&]( const Value& v ) { result.push_back( v ); } );
        return result;
    }

    template< class Value >
    inline auto is_sorted( const Joinable_list_<Value>& list )
        -> bool
    {
        const Value*    previous    = nullptr;
        bool            result      = true;
        
        list.for_each( [&]( const Value& v )
        {
            if( previous and *previous > v ) {
                result = false;
            }
            previous = &v;
        } );

        return result;
    }

}  // namespace techniques
