// File: global.h
#ifndef GLOBAL_H
#define GLOBAL_H

#include <GL/freeglut.h>
#include "Utils.h"
#include "SingletonView.h"
#include "Light.h"

namespace Global
{    
    using namespace mygllib;
    
    extern GLuint texture[3];
    extern float yangle;
    extern Light light;
    extern int y_axis_angle;
    extern float xangle;
    extern int id;
    extern int env;

    
}

#endif
