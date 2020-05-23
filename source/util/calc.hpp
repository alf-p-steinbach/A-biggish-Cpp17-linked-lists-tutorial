#pragma once
#include <limits>           // std::numeric_limits

namespace util::calc
{
    namespace _alias{ namespace calc = util::calc; }
    using std::numeric_limits;

    constexpr double nan = numeric_limits<double>::quiet_NaN();
    
    inline auto percent( const double a, const double b )
        -> double
    { return 100*(b - a)/a; }
}  // namespace util::calc
