#include <iostream>
using std::cout, std::endl;

struct Node
{
    Node*       pointer_to_next;
    double      value;
};

class List
{
    Node minus1      = { nullptr, -1 };
    Node the_answer  = { &minus1, 42 };
    Node zero        = { &the_answer, 0 };
    Node e           = { &zero, 2.72 };
    Node pi          = { &e, 3.14 };
    
public:
    class Const_iterator        // With just the operations actually used here.
    {
        Node const*     m_pointer;

    public:
        auto operator*() const
            -> double
        { return m_pointer->value; }
        
        auto operator++()
            -> Const_iterator&
        {
            m_pointer = m_pointer->pointer_to_next;
            return *this;
        }
        
        friend auto operator!=(
            const Const_iterator& a, const Const_iterator& b
            ) -> bool
        { return a.m_pointer != b.m_pointer; }
        
        explicit Const_iterator( Node const* const p ):
            m_pointer( p )
        {}
    };
    
    auto start() const
        -> Const_iterator
    { return Const_iterator( &pi ); }
    
    auto beyond() const
        -> Const_iterator
    { return Const_iterator( nullptr ); }
};

auto main()
    -> int
{
    List const list = {};

    // Traverse the list & output the values:
    for( auto it = list.start();  it != list.beyond(); ++it ) {
        cout << *it << endl;
    }
}
