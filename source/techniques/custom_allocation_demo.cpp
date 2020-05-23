#include "../Type_.hpp"

#include <stddef.h>         // size_t

#include <iostream>         // std::(cout, endl)
using std::cout, std::endl;

struct Whatever
{
    char filler[1234];

    // Allocation.
    static auto operator new( const size_t size )
        -> void*
    {
        cout << "Allocated " << size << " bytes for an object." << endl;
        return ::operator new( size );
    }
    
    // Deallocation.
    static void operator delete( const Type_<void*> p, const size_t size )
    {
        cout << "Deallocated a " << size << " bytes chunk." << endl;
        ::operator delete( p );
    }
    
    // Construction.
    Whatever() { cout << "  Constructed a Whatever object." << endl; }
    
    // Destruction.
    ~Whatever() { cout << "  Destroyed a Whatever object." << endl; }
};

auto main()
    -> int
{
    auto p = new Whatever;      // new-expression: calls alloc, then constructor.
    delete p;                   // delete-expression: calls destructor, then dealloc.
}
