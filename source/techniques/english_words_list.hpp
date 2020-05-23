#pragma once
#include "../data/english_words_iteration.hpp"
#include "Joinable_list_.hpp"

namespace techniques {
    using std::string_view;
    
    inline auto english_words_list()
        -> Joinable_list_<string_view>
    {
        Joinable_list_<string_view> result_list;
        data::for_each_english_word( [&]( const string_view& word )
        {
            result_list.push_back( word );
        } );
        return result_list;
    }

}  // namespace oneway_sorting_examples
