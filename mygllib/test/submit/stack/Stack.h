#ifndef STACK_H
#define STACK_H

#include "SLList.h"

template < typename T >
class Stack
{
public:
    void push(const T & key)
    {
        list_.insert_head(key);
    }
    void pop()
    {
        list_.remove_head();
    }
    T & top()
    {
        return list_.head()->key();
        
    }
    T top() const
    {
        return list_.head()->key();
    }
    int size() const
    {
        return list_.size();
    }
    bool empty() const
    {
        return list_.is_empty();
    }
    void clear()
    {
        list_.clear();
    }
private:
    SLList< T > list_;
};

#endif
