#ifndef SLNODE_H
#define SLNODE_H

#include <iostream>

template < typename T >
class SLNode
{
public:
    SLNode(const T & key = 0, SLNode * next=NULL)
        : key_(key), next_(next)
    {}

    SLNode(const SLNode & node)
        : key_(node.key()), next_(NULL)
    {}

    SLNode(const SLNode< T > * node)
        : key_(node->key()), next_(NULL)
    {} 
    
    T get_key() const
    {
        return key_;
    }

    T key() const
    {
        return key_;
    }
    T & key()
    {
        return key_;
    }
    void set_key(const T & key)
    {
        key_ = key;
    }
    
    SLNode * get_next() const
    {
        return next_;
    }

    void set_next(SLNode< T > * node)
    {
        next_ = node;
    }
    static void debug_printing(bool);
    static bool debug()
    {
        return debug_;
    }
private:
    T key_;
    SLNode * next_;
    static bool debug_;
};

template< typename T >
bool SLNode< T >::debug_ = false;

template< typename T >
void SLNode< T >::debug_printing(bool debug)
{
    debug_ = debug;
}

template < typename T >
std::ostream & operator<<(std::ostream & cout, const SLNode< T > & node)
{
    if (SLNode< T >::debug())
    {
        cout << "<SLNode " << &node << " key:" << node.get_key() << ", next:" << node.get_next() << '>';
    }
    else
        cout << node.key();
    return cout;
}

#endif
