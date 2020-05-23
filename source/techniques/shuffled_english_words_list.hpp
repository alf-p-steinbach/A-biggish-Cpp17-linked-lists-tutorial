#pragma once
#include "../my_random.hpp"

#include "english_words_list.hpp"
#include "merge_shuffle.hpp"

namespace techniques {
    using my_random::Seed;
    
    inline auto shuffled_english_words_list( const Seed seq_nr = Seed( 42 ) )
        -> Joinable_list_<string_view>
    {
        auto result = english_words_list();
        merge_shuffle( result, seq_nr );
        return result;
    }

}  // namespace techniques
