#ifndef DLNODE_H
#define DLNODE_H

#include <iostream>

template< typename T >
class DLNode
{
public:
    DLNode(DLNode * prev = NULL, DLNode * next = NULL)
        : prev_(prev), next_(next)
    {}
    DLNode(const T & key, DLNode * prev = NULL, DLNode * next = NULL)
        : key_(key), prev_(prev), next_(next)
    {}
    T key() const 
    {
        return key_;
    }
    T & key()
    {
        return key_;
    }
    DLNode * next() const
    {
        return next_;
    }
    DLNode *& next()
    {
        return next_;
    }
    DLNode * prev() const
    {
        return prev_;
    }
    DLNode *& prev()
    {
        return prev_;
    }
    static void debug_printing(bool);
    static bool debug()
    {
        return debug_;
    }
    
private:
    T key_;
    DLNode * next_;
    DLNode * prev_;
    static bool debug_;
};


template< typename T >
bool DLNode< T >::debug_ = false;

template< typename T >
void DLNode< T >::debug_printing(bool debug)
{
    debug_ = debug;
}

template < typename T >
std::ostream & operator<<(std::ostream & cout, const DLNode< T > & node)
{
    if (DLNode< T >::debug())
    {
        cout << "<DLNode " << &node << " prev:" << node.prev() << " key:" << node.key()
             << ", next:" << node.next() << '>';
    }
    else
        cout << node.key();
    return cout;
}

#endif
