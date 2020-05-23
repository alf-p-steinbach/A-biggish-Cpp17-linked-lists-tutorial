#pragma once
#include "Node.hpp"

#include <assert.h>     // assert

#include <type_traits>  // std::aligned_storage_t
#include <vector>       // std::vector

namespace oneway_sorting_examples {
    using std::aligned_storage_t, std::vector;
    
    using Node_memory = aligned_storage_t< sizeof(Node), alignof(Node) >;
    
    class Block_allocator final:
        public Node_allocator
    {
        vector<Node_memory>     m_chunks;
        Size                    m_n_chunks_in_use = 0;
        
        Block_allocator() {}

    public:
        void reserve( const Size n_chunks )
        {
            m_chunks.reserve( n_chunks );
        }

        auto allocate( const Size n_bytes )
            -> void* override
        {
            assert( n_bytes == sizeof( Node ) );
            m_chunks.emplace_back();
            ++m_n_chunks_in_use;
            return &m_chunks.back();
        }

        void deallocate( const Type_<void*> )
            override
        {
            // Do nothing about reusing each chunk. Assume calling code is correct.
            --m_n_chunks_in_use;
            assert( m_n_chunks_in_use >= 0 );
            if( m_n_chunks_in_use == 0 ) {
                m_chunks.resize( 0 );
            }
        }

        static auto instance()
            -> Block_allocator&
        {
            static Block_allocator the_instance;
            return the_instance;
        }
    };

    inline auto Node_allocator::instance()
        -> Node_allocator&
    { return Block_allocator::instance(); }

}  // namespace oneway_sorting_examples
