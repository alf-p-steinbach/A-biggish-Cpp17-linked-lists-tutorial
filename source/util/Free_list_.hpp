#pragma once
#include "signed_size_types.hpp"
#include "Type_.hpp"

#include <assert.h>         // assert
#include <type_traits>      // std::is_final_v

namespace memory {
    using std::is_final_v;
    using signed_size_types::Size;

    template< class T > constexpr bool is_final_class_ = is_final_v<T>;

    template< class Object >
    class Free_list_
    {
        static_assert( is_final_class_<Object>, "!" );

        Free_list_( const Free_list_& ) = delete;
        auto operator=( const Free_list_& ) -> Free_list_& = delete;

        struct Linkable { Linkable* next; };
        Linkable*   m_head;
        Size        m_count;
        Size        m_max_count;

        Free_list_():
            m_head( nullptr ),
            m_count( 0 ),
            m_max_count( 4'000'000'000 / sizeof( Object ) )
        {}

        auto unlinked_head()
            -> Linkable*
        {
            const auto result = m_head;
            m_head = m_head->next;
            --m_count;
            return result;
        }

    public:
        static auto instance()
            -> Free_list_&
        {
            static Free_list_ the_instance;
            return the_instance;
        }

        ~Free_list_()
        {
            while( m_head ) {
                ::operator delete( unlinked_head() );
            }
        }

        auto count() const
            -> Size
        { return m_count; }

        auto allocate( const Size n_bytes )
            -> void*
        {
            assert( n_bytes >= Size( sizeof( Object ) ) );

            if( not m_head ) {
                return ::operator new( n_bytes );
            }

            return unlinked_head();
        }
        
        void deallocate( const Type_<void*> p )
        {
            if( m_count >= m_max_count ) {
                ::operator delete( p );
            } else {
                m_head = ::new( p ) Linkable{ m_head };
                ++m_count;
            }
        }
        
        void shrink_to_fit()
        {
            while( m_count > m_max_count ) {
                ::operator delete( unlinked_head() );
            }
        }

        auto max_count() const
            -> Size
        { return m_max_count; }

        void set_max_count( const Size n )
        {
            m_max_count = n;
        }
    };

}  // namespace memory
