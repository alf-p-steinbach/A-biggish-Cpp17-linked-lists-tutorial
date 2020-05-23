#include "common/display_timing_results.hpp"
#include "merge_sort_recursively.hpp"
namespace x = oneway_sorting_examples;
using   namespace x::common::_alias;
using   x::merge_sort_recursively;

auto main()
    -> int
{
    common::display_timing_results( "Recursive merge-sort", merge_sort_recursively );
}
