#include <stdlib.h>         // EXIT_...

#include <assert.h>
#include <forward_list>     // std::forward_list
#include <iostream>
#include <string>           // std::(stoi)
#include <utility>          // std::move
using std::forward_list, std::cout, std::cerr, std::endl, std::stoi, std::move;

auto is_even( const int x )
    -> bool
{ return x % 2 == 0; }

auto reversed_collatz_sequence( const int x, forward_list<int> earlier_part = {} )
    -> forward_list<int>
{
    earlier_part.push_front( x );
    if( x == 1 ) {
        return earlier_part;
    }
    const int next_x = (is_even( x )? x/2 : 3*x + 1);
    return reversed_collatz_sequence( next_x, move( earlier_part ) );
}

auto collatz_sequence( const int x )
    -> forward_list<int>
{
    forward_list<int> result = reversed_collatz_sequence( x );
    result.reverse();
    return result;
}

auto main( int n_command_parts, char** command_parts )
    -> int
{
    if( n_command_parts != 2 ) {
        cerr << "!Usage: " << command_parts[0] << " STARTING_NUMBER" << endl;
        return EXIT_FAILURE;
    }
    const int starting_number = stoi( command_parts[1] );
    assert( starting_number >= 1 );

    for( const int v: collatz_sequence( starting_number ) ) {
        if( v != starting_number ) {        // We trust mr. Collatz.
            cout << " -> ";
        }
        cout << v;
    }
    cout << "." << endl;
}
