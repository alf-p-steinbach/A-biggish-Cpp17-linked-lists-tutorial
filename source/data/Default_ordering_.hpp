#pragma once

#include <algorithm>        // std::max
#include <string>           // std::string
#include <string_view>      // std::string_view
#include <type_traits>      // std::common_type

namespace data {
    using std::common_type, std::max, std::string, std::string_view;

    template< class Value >
    struct Default_ordering_
    {
        using Avg_result = common_type<Value, double>;
        
        static auto compare( const Value& a, const Value& b )
            -> int
        { return (a < b? -1 : a > b? +1 : 0); }

        static auto average_of( const Value a, const Value b )
            -> Avg_result
        { return a/2.0 + b/2.0; }
    };

    template<>
    struct Default_ordering_<string_view>
    {
        using Avg_result = string;

        static auto compare( const string_view& a, const string_view& b )
            -> int
        { return a.compare( b ); }

        static auto average_of( const string_view& a, const string_view& b )
            -> Avg_result
        {
            using Byte = unsigned char;
            constexpr int bits_per_byte = CHAR_BIT;

            string result;
            const int n_codes = static_cast<int>( max( a.length(), b.length() ) );
            result.resize( n_codes + 1 );
            
            // result = a + b;
            unsigned carry = 0;
            for( int i = n_codes - 1;  i >= 0; --i ) {
                const unsigned code_a = (i >= int( a.size() )? 0u : Byte( a[i] ));
                const unsigned code_b = (i >= int( b.size() )? 0u : Byte( b[i] ));
                const unsigned sum = code_a + code_b + carry;
                result[i] = char( sum & Byte( -1 ) );
                carry = sum >> bits_per_byte;
            }
            
            // result /= 2;
            for( int i = 0; i < n_codes + 1; ++i ) {
                const int new_msb = carry;
                carry = Byte( result[i] )%2;
                result[i] = char( Byte( result[i] )/2 + (new_msb << (bits_per_byte - 1)) );
            }
            if( result[n_codes] == 0 ) {
                result.resize( n_codes );
            }
            return result;
        }
    };

}  // namespace data
