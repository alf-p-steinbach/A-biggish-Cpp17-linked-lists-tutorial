#pragma once
#include "../data/english_words_iteration.hpp"
#include "List.hpp"

namespace oneway_sorting_examples {
    
    inline auto english_words_list()
        -> List
    {
        List list;
        List::Appender appender( list.head );
        data::for_each_english_word( [&]( const string_view& word )
        {
            appender.append( new Node{ nullptr, word } );
        } );
        return list;
    }

}  // namespace oneway_sorting_examples
