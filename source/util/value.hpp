#pragma once
// Workaround for Visual C++ 2019 bug, reported at
// <url: https://developercommunity.visualstudio.com/content/problem/942090/index.html>
//
// With MSVC 2019, `+v` or `0+v` is an rvalue expression with same `const`-ness as `v`.
// To support that compiler, instead of `+v` or `0+v` write `value( v )`.

namespace util {

    template< class T >
    inline auto value( const T& v )
        -> T
    { return v; }

}  // namespace util
