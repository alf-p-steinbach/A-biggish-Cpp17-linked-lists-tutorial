#include "../my_random.hpp"

#include <iostream>
#include <string>

auto main() -> int
{
    using std::cout, std::endl, std::string;

    my_random::Choices choices;
    
    for( int j = 1; j <= 10; ++j ) {
        string s;
        for( int i = 1; i <= 64; ++i ) { s += (choices.next()? '1' : ' '); }
        cout << ">" << s << "<" << endl;
    }
}
