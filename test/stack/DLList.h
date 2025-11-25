#ifndef DLLIST_H
#define DLLIST_H

#include <string>
#include "DLNode.h"

template< typename T >
class DLList
{
public:
    
    class iterator;
    class const_iterator;

    class iterator
    {
    public:
        iterator(DLNode<T> * p )
            : p_(p)
        {}
        iterator(const iterator & p)
            : p_(p.p_)
        {}
        const iterator & operator++()
        {
            p_ = p_->next();
            return *this;
        }
        const bool operator!=(const iterator & p) const
        {
            return p_ != p.p_;
        }
        const T operator*() const
        {
            return p_->key();
        }
    private:
        DLNode<T> * p_;
    };

    DLList()
        : pheadsentinel_(new DLNode< T >()),
          ptailsentinel_(new DLNode< T >(pheadsentinel_))
    {
        pheadsentinel_->next() = ptailsentinel_;
    }
    DLList(const DLList & list)
        : pheadsentinel_(new DLNode< T >),
          ptailsentinel_(new DLNode(pheadsentinel_))
    {
        
        DLNode< T > * p = list.head();
        while (p != ptailsentinel_)
        {
            insert_tail(p->key());
            p = p->next();
        }
    }
    ~DLList()
    {
        clear();
        delete pheadsentinel_;
        delete ptailsentinel_;
    }
    int size() const
    {
        DLNode< T > * p = head();
        int s = 0;
        while (p != ptailsentinel_)
        {
            ++s;
            p = p->next();
        }
        return s; 
    }
    DLList & operator=(const DLList & list)
    {
        DLNode< T > * p = list.head();
        while (p != ptailsentinel_)
        {
            insert_tail(p->key());
            p = p->next();
        }
        return *this;
    }
    
    DLNode< T > * head() const
    {
        return pheadsentinel_->next();
    }

    DLNode< T > *& head()
    {
        return pheadsentinel_->next();
    }
    
    DLNode< T > * tail() const
    {
        return ptailsentinel_->prev();
    }

    DLNode< T > *& tail()
    {
        return ptailsentinel_->prev();
    }

    DLNode< T > * find(const T & key) const
    {
        DLNode< T > * p = head();
        while (p != ptailsentinel_)
        {
            if (p->key() == key)
            {
                return p;
            }
            p = p->next();
        }
        
        return NULL;
    }
    
    void clear()
    {
        while (pheadsentinel_->next() != ptailsentinel_)
        {
            delete_head();
        }
    }
    bool is_empty() const
    {
        return (pheadsentinel_->next() == ptailsentinel_);
    }
    void insert_after(DLNode< T > * p, const T & key)
    {
        DLNode< T > * q = new DLNode(key, p, p->next());
        p->next() = q;
        q->next()->prev() = q;
    }
    void insert_before(DLNode< T > * p, const T & key)
    {
        insert_after(p->prev(), key);
    }
    void insert_head(const T & key)
    {
        insert_after(pheadsentinel_, key);
    }
    void push_front(const T & key)
    {
        insert_after(pheadsentinel_, key);
    }
    void insert_tail(const T & key)
    {
        insert_before(ptailsentinel_, key);
    }
    void push_back(const T & key)
    {
        insert_before(ptailsentinel_, key);
    }
    void delete_at(DLNode< T > * p)
    {
        p->prev()->next() = p->next();
        p->next()->prev() = p->prev();
        delete p;
    }
    void delete_head()
    {
        delete_at(pheadsentinel_->next());
    }
    void delete_tail()
    {
        delete_at(ptailsentinel_->prev());
    }

    iterator begin()
    {
        return iterator(pheadsentinel_->next());
    }
    iterator end()
    {
        return iterator(ptailsentinel_->prev());
    }
    
private:
    DLNode< T > * pheadsentinel_, * ptailsentinel_;
};

template < typename T >
std::ostream & operator<<(std::ostream & cout, const DLList< T > & list)
{
    DLNode< T > * p = list.head();
    if (DLNode< T >::debug())
    {
        cout << "\nDLList" << &list << " phead:" << p
             << " ptail:" << list.tail() << std::endl;
        while (p->next() != NULL)
        {
            std::cout << "    " << *p << std::endl;
            p = p->next();
        }
        std::cout << '>';
    }
    else
    {
        std::string delim = "";
        cout << '[';
        while (p->next() != NULL)
        {
            std::cout << delim << *p;
            delim = ", ";
            p = p->next();
        }
        cout << ']';
    }
    
    return cout;
}

#endif
