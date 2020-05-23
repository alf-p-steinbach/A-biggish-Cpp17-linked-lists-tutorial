#pragma once
#include <algorithm>        // std::(min, max)
#include <utility>          // namespace std::rel_ops

namespace util {
    template< class Derived >
    struct Number_like_
    {
        friend auto operator+( const Derived& a, const Derived& b )
            -> Derived
        { return Derived( a.numerical() + b.numerical() ); }

        friend auto operator-( const Derived& a, const Derived& b )
            -> Derived
        { return Derived( a.numerical() - b.numerical() ); }

        friend auto operator*( const int n, const Derived& x )
            -> Derived
        { return Derived( n*x.numerical() ); }

        friend auto operator/( const Derived& x, const int n )
            -> Derived
        { return Derived( x.numerical()/n ); }

        friend auto operator<( const Derived& a, const Derived& b )
            -> bool
        { return a.numerical() < b.numerical(); }

        friend auto operator==( const Derived& a, const Derived& b )
            -> bool
        { return a.numerical() == b.numerical(); }

        friend auto min( const Derived& a, const Derived& b )
            -> Derived
        { using std::min; return Derived( min( a.numerical(), b.numerical() ) ); }

        friend auto max( const Derived& a, const Derived& b )
            -> Derived
        { using std::max; return Derived( max( a.numerical(), b.numerical() ) ); }
    };
}  // namespace util
