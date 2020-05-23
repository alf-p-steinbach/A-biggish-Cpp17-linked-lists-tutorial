#pragma once
#include "../util/std_random.hpp"

#include "english_words_list.hpp"
#include "merge_shuffle.hpp"

namespace oneway_sorting_examples {
    using namespace util::std_random::_alias;
    
    inline auto shuffled_english_words_list(
        const std_random::Seed seed = std_random::Seed( 42 )
        ) -> List
    {
        List result = english_words_list();
        merge_shuffle( result, seed );
        return result;
    }

}  // namespace oneway_sorting_examples
