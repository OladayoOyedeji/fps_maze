#include <cmath>
#include <iostream>
#include <GL/freeglut.h>
#include "gl3d.h"
#include "View.h"
#include "SingletonView.h"
#include "Keyboard.h"
#include "Reshape.h"
#include "Material.h"
#include "Light.h"
#include "global.h"
#include "Maze.h"
#include "GameObject.h"
#include "GameGlobal.h"

#include "ImageFile.h"

using namespace mygllib;


// GLuint texture[3];        // array of texture indices
// float yangle = 0.0;       // angle about y-axis to rotate textured square
// float xangle = 0.0;       // angle about x-axis to rotate textured square
// int id = 0;               // currently displayed texture id

// int env = 0;

using namespace Global;

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    {
        mygllib::Light::all_off();
        glDisable(GL_TEXTURE_2D);
        mygllib::draw_xz_plane();
        mygllib::draw_axes();
        mygllib::Light::all_on();
        glPushMatrix();
        {
            // Util::bu
            
            maze.draw_maze(person.x_, person..z_);
            person.draw_object();
            //enemy.draw_enemy();
            
            for (int i = 0; i < enemies.size(); ++i)
            {
                enemies[i]->draw_enemy();
            }
        }
        glPopMatrix();
    }
    glPopMatrix();
    glutSwapBuffers();	
}

void animate(int somevalue)
{
    
    //enemy.run();

    for (int i = 0; i < enemies.size(); ++i)
    {
        enemies[i]->run();
    }
    // check_collision_with_wall(person, punched_walls);
    
    glutPostRedisplay();
    glutTimerFunc(1, animate, 1);
}

// void specialkeyboard(int key, int x, int y)
// {
//     mygllib::View & view = *(mygllib::SingletonView::getInstance());
//     bool reset = false;
//     switch (key)
//     {
//         case GLUT_KEY_LEFT:
//             yangle -= 5.0;
//             if (yangle < 0.0) yangle += 360.0;
//             reset = true;
//             break;
//         case GLUT_KEY_RIGHT: 
//             yangle += 5.0;
//             if (yangle > 360.0) yangle -= 360.0;
//             reset = true;
//             break;
//         case GLUT_KEY_UP:
//             xangle -= 5.0;
//             if (xangle < 0.0) xangle += 360.0;
//             reset = true;
//             break;
//         case GLUT_KEY_DOWN: 
//             xangle += 5.0;
//             if (xangle > 360.0) xangle -= 360.0;
//             reset = true;
//             break;
//     }
//     if (reset)
//     {
//         view.lookat();
//         glutPostRedisplay();
//     }
// }


int main(int argc, char **argv) 
{
    std::cout << "left/right arrow: rotate" << '\n'
              << "space: toggle textures" << '\n'
              << "delete: reset" << '\n';
    
    mygllib::init3d();
    init();
    
    glutDisplayFunc(display);
    glutReshapeFunc(mygllib::Reshape::reshape);
    glutKeyboardFunc(mygllib::Keyboard::keyboard);
    //glutSpecialFunc(specialkeyboard);
    glutTimerFunc(1, animate, 1);
    glutMainLoop();
    
    return 0; 
}
