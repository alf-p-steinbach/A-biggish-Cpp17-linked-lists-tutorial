#pragma once
#include "../my_random.hpp"

#include "english_words_list.hpp"
#include "merge_shuffle.hpp"

namespace oneway_sorting_examples {
    using my_random::Seed;
    
    inline auto shuffled_english_words_list( const Seed seq_nr = Seed( 42 ) )
        -> List
    {
        List result = english_words_list();
        merge_shuffle( result, seq_nr );
        return result;
    }

}  // namespace oneway_sorting_examples
