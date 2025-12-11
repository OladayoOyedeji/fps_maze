// File  : SingletonView.h
// Author: smaug

#ifndef SINGLETONVIEW_H
#define SINGLETONVIEW_H

#include "View.h"


namespace mygllib
{
    class SingletonView
    {
    public:
        static View * getInstance();
    private:
        static View * instance_;
    };
}

#endif
