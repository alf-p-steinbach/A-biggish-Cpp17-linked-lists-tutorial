#include <stddef.h>     // ptrdiff_t, size_t

#include <stdexcept>    // std::out_of_range
#include <string>       // std::string
#include <type_traits>  // std::aligned_storage_t
#include <vector>       // std::vector

namespace basics {
    using Size = ptrdiff_t;     // A signed size type, the type of C++20 std::ssize().
    using Index = Size;
    using Unsigned_size = size_t;
    static_assert( sizeof( Unsigned_size ) == sizeof( Size ) ); // No 16-bit systems now.

    using C_str = const char*;

    inline auto hopefully( const bool expr )
        -> bool
    { return expr; }
    
    template< class X >
    inline auto fail_( const string& msg )
        -> bool
    { throw X( msg ); }
}  // namespace basics

namespace my {
    using
        std::aligned_storage_t, std::string, std::out_of_range;
    using
        Basics::Index, Basics::Unsigned_size, Basics::C_str,
        Basics::hopefully, Basics::fail_;

    struct Capacity { Size value; };

    struct Unchecked{};
    constexpr auto unchecked = Unchecked();

    template< class Item >
    class Array_list
    {
        using Item_storage = aligned_storage_t<Item>;

        vector<Item_storage>    m_items;
        Index                   m_eindex_gap_start;         // "eindex" -> external index.
        Index                   m_gap_size;
        Index                   m_index_offset;

        auto index_of_external( const Index i ) const
            -> Index
        {
            const Index gap_adjusted_ei = (i < m_eindex_gap_start? i : i + m_gap_size);
            return (gap_adjusted_ei + i_offset) % m_items.size();
        }

        void assert_eindex_is_valid( const Index i, const C_str funcname ) const
        {
            using S = string;
            hopefully( Unsigned_size( i ) < size() )
                or fail_<out_of_range>( S() + funcname + " - index out of range" );
        }

    public:
        auto size() const       -> Size     { return m_items.size() - m_gap_size; }
        auto capacity() const   -> Size     { return m_items.size(); }
        auto position() const   -> Index    { return m_eindex_gap_start; }

        auto item( Unchecked, const Index i ) const
            -> const Item&
        { return m_items[index_of_external( i )]; }

        auto item( Unchecked, const Index i )
            -> Item&
        { return m_items[index_of_external( i )]; }

        auto item( const Index i ) const
            -> const Item&
        {
            assert_eindex_is_valid( i, __func__ );
            return item( unchecked, i );
        }

        auto item( const Index i )
            -> Item&
        {
            assert_eindex_is_valid( i, __func__ );
            return item( unchecked, i );
        }

        Array_list( const Capacity capacity = {0} ):
            m_items(),
            m_i_gap_start( 0 ),
            m_i_gap_size( 0 ),
            m_i_current( 0 )
        { m_items.reserve( capacity.value ); }

        Array_list(
            const Size      n,
            const Item&     default_value   = {},
            const Capacity  capacity        = {0}
            ):
            m_items( n, default_value ),
            m_i_gap_start( 0 ),
            m_i_gap_size( 0 ),
            m_i_current( 0 )
        { m_items.reserve( capacity.value ); }
    };

}  // namespace my
