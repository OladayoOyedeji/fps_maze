// File  : Keyboard.cpp
// Author: smaug

#include <GL/freeglut.h>
#include "View.h"
#include "SingletonView.h"
#include "Keyboard.h"
#include "global.h"
#include "GameGlobal.h"

using namespace Global;

void mygllib::Keyboard::keyboard(unsigned char key, int w, int h)
{
    //using namespace mygllib;
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    bool reset = false;
     switch(key) 
    {
        
        // case 's': person.look_y_plane(-0.1); reset = true; break;
        // case 'd': person.look_xz_plane(-0.05); reset = true; break;
        
        case 'w': person.move_xz_plane(0.5); reset = true; break;
        case 'm': maze.mode = !maze.mode; reset = true; break;
            //case 'u': enemy.run(); reset = true; break;
            
        case 'x': view.eyex() -= 0.1; reset = true; break;
        case 'X': view.eyex() += 0.1; reset = true; break;
        case 'y': view.eyey() -= 0.1; reset = true; break;
        case 'Y': view.eyey() += 0.1; reset = true; break;
        case 'z': view.eyez() -= 0.1; reset = true; break;
        case 'Z': view.eyez() += 0.1; reset = true; break;
            
        case 'r': y_axis_angle += 1; reset = true; break;
        case 'R': y_axis_angle -= 1; reset = true; break;
        
        case '1': light.x() += 0.1; reset = true; break;
        case '2': light.x() -= 0.1; reset = true; break;
        case '3': light.y() += 0.1; reset = true; break;
        case '4': light.y() -= 0.1; reset = true; break;
        case '5': light.z() += 0.1; reset = true; break;
        case '6': light.z() -= 0.1; reset = true; break;
        case '7': person.change_view(); reset = true; break;
    
        case ' ':
            // ++id; id = (id > 2 ? 0 : id);
            // reset = true;
            person.shoot();
            break;
        case 127: // delete key
            xangle = 0.0;
            yangle = 0.0;
            reset = true;
            break;

        case 'e':
            env = (env == 0 ? 1 : 0);
            reset = true;
            break;
        default:
            break;
    }
    if (reset)
    {
        person.update_eye_ref(view);
        view.set_projection();
        view.lookat();
        glutPostRedisplay();
    }
    
}
