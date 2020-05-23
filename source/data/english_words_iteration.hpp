#include "english_words.hpp"

#include <string_view>      // std::string_view

namespace data {
    using std::string_view;
    
    template< class Func >
    inline void for_each_english_word( const Func& f )
    {
        const string_view&  s           = english_words;
        const int           s_length    = int( s.length() );

        struct Word{ int i_start; int i_end; };
        
        Word word = {};
        while( word.i_start < s_length ) {
            word.i_end = word.i_start + 1;
            while( word.i_end < s_length and s[word.i_end] != '\n' ) {
                ++word.i_end;
            }
            const int   word_length = word.i_end - word.i_start;
            const auto  word_view   = string_view( &s[word.i_start], word_length );

            f( word_view );

            word.i_start = word.i_end + 1;
        }
    }
}  // namespace data
