#ifndef SLLIST_H
#define SLLIST_H

#include "SLNode.h"
#include <string>

class IndexError
{};

class ValueError
{};

template < typename T >
class SLList
{
public:
    SLList()
        : phead_(NULL)
    {}
    SLList(const SLList & list)
        : phead_(new SLNode(list.phead_->key()))
    {
        SLNode< T > * q = list.phead_->get_next();
        while (q != NULL)
        {
            insert_tail(q->key());
            q = q->get_next();
        }
    }
    ~SLList()
    {
        
        clear();
    }
    SLList & operator=(const SLList & list)
    {
        clear();
        phead_ = new SLNode(list.phead_->key());
        SLNode< T > * q = list.phead_->get_next();
        while (q != NULL)
        {
            insert_tail(q->key());
            q = q->get_next();
        }
        return *this;
    }
    void clear()
    {
        while (phead_ != NULL)
        {
            remove_head();
        }
        phead_ = NULL;
    }
    SLNode< T > * head() const
    {
        return phead_;
    }
    SLNode< T > *& head()
    {
        return phead_;
    }
    void insert_head(const T & key)
    {
        phead_ = new SLNode(key, phead_);
    }
    void insert_tail(const T & key)
    {
        SLNode< T > * p = phead_;
        if (p == NULL)
        {
            insert_head(key);
        }
        else
        {
            while (p->get_next() != NULL)
            {
                p = p->get_next();
            }
            p->set_next(new SLNode(key));
        }
    }
    T remove_head()
    {
        SLNode< T > * p = phead_->get_next();
        //std::cout << p << std::endl;
        T key = phead_->key();
        delete phead_;
        phead_ = p;
        return key;
    }
    T remove_tail()
    {
        SLNode< T > * p = phead_->get_next();
        while (p->get_next()->get_next() != NULL)
        {
            p = p->get_next();
        }
        SLNode< T > * q = p->get_next();
        T key = q->key();
        delete q;
        p->set_next(NULL);
        return key;
        
    }
    SLNode< T > * find(const T &key)
    {
        SLNode< T > * p = phead_;
        while (p != NULL)
        {
            if (p->key() == key)
            {
                return p;
            }
            p = p->get_next();
        }
        return NULL;
    }

    bool is_list(SLNode< T > * p)
    {
        SLNode<T> * q = phead_;
        while (q != NULL)
        {
            if (q == p)
            {
                return true;
            }
            q = q->get_next();
        }
        return false;
    }

    T remove(SLNode< T > * p)
    {
        if (!is_list(p))
        {
            throw ValueError();
        }
        T key = p->key();
        if (p == phead_)
        {remove_head(); return key;}
        SLNode< T > * next = p->get_next();
        SLNode< T > * prev = phead_;
        while (prev->get_next() != p)
        {
            prev = prev->get_next();
        }
        prev->set_next(next);
        delete p;
        return key;
        
    }

    void remove(const T & key)
    {
        SLNode< T > * p = find(key);
        if (!is_list(p))
        {
            throw ValueError();
        }
        SLNode< T > * next = p->get_next();
        SLNode< T > * prev = phead_;
        while (prev->get_next() != p)
        {
            prev = prev->get_next();
        }
        prev->set_next(next);
        delete p;
        
    }
    
    T operator[](int n) const
    {
        SLNode< T > * p = phead_;
        int i = 0;
        while (i < n)
        {
            p = p->get_next();
            i++;
            if (p->get_next() == NULL)
                break;
        }
        if (i != n)
        {
            throw IndexError();
        }
        return p->key();
    }
    bool is_empty() const
    {
        return (phead_ == NULL);
    }

    int size() const
    {
        int size_ = 0;
        SLNode< T > * p = phead_;
        while (p != NULL)
        {
            p = p->get_next();
            size_++;
        }
        return size_;
    }
    
    static void debug_printing(bool);
    static bool debug()
    {
        return debug_;
    }
  private:
    SLNode< T > * phead_;
    static bool debug_;
};

template< typename T >
bool SLList< T >::debug_ = false;

template< typename T >
void SLList< T >::debug_printing(bool debug)
{
    debug_ = debug;
}

template < typename T >
std::ostream & operator<<(std::ostream & cout, const SLList< T >& list)
{
    SLNode< T > * p = list.head();
    if (SLNode< T >::debug())
    {
        std::cout << "\n<SLList " << &list << " phead:"
                  << p << std::endl;
        while (p != NULL)
        {
            std::cout << "   " << *p << std::endl;
            p = p->get_next();
        }
        std::cout << '>';
    }
    else
    {
        std::string delim = "";
        std::cout << '[';
        while (p != NULL)
        {
            std::cout << delim << *p;
            delim = ", ";
            p = p->get_next();
        }
        std::cout << ']';
    }
    return cout;
}

#endif
