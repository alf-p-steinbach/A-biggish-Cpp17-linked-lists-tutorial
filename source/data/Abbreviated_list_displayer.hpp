#pragma once
#include <ostream>          // std::ostream
#include <string_view>      // std::string_view

namespace data {
    using std::ostream, std::string_view;

    class Abbreviated_list_displayer    // Non-copyable
    {
        ostream&        m_stream;
        int             m_n_items;
        int             m_index;

    public:
        void display( const string_view& item )
        {
            if( m_index < 5 or m_n_items - 5 <= m_index ) {
                if( m_index > 0 ) {
                    m_stream << ", ";
                }
                if( m_index == m_n_items - 5 ) {
                    m_stream << "..., ";
                }
                m_stream << item;
            }
            ++m_index;
        }
        
        Abbreviated_list_displayer( ostream& stream, const int n_items ):
            m_stream( stream ),
            m_n_items( n_items ),
            m_index( 0 )
        {}
    };
}  // namespace data
