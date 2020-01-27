#include "english_words.hpp"

#include <string_view>      // std::string_view

namespace data {
    using std::string_view;
    
    template< class Func >
    inline void for_each_english_word( const Func& f )
    {
        const string_view&  s           = english_words;
        const int           s_length    = int( s.length() );

        int i_wordstart = 0;
        while( i_wordstart < s_length ) {
            int i_wordend = i_wordstart + 1;
            while( i_wordend < s_length and s[i_wordend] != '\n' ) {
                ++i_wordend;
            }
            const int   word_length = i_wordend - i_wordstart;
            const auto  word        = string_view( &s[i_wordstart], word_length );

            f( word );

            i_wordstart = i_wordend + 1;
        }
    }

}  // namespace data
