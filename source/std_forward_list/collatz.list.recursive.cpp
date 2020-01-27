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

auto cons( const int x, forward_list<int>&& list )
    -> forward_list<int>
{
    list.push_front( x );
    return move( list );
}

auto collatz_sequence( const int x )
    -> forward_list<int>
{
    forward_list<int> tail;
    if( x > 1 ) {
        tail = collatz_sequence( is_even( x )? x/2 : 3*x + 1 );
    }
    return cons( x, move( tail ) );
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
