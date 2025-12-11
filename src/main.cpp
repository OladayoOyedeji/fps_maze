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

            // std::cout << "direction " << person.dir_[0] << ' ' << person.dir_[1] << std::endl;
            int dir_x = person.dir_[0] / abs(person.dir_[0]);
            int dir_z = person.dir_[1] / abs(person.dir_[1]);
            maze.draw_maze(person.x_, person.z_, dir_x, dir_z);
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
        // for (auto p: person.amo)
        auto p = person.amo.begin();
        while (p != person.amo.end())
        {
            if (enemies[i]->collide_object(*p))
            {
                
                std::cout << "colision\n" << std::endl;
                enemies[i]->state_ = -1;
                auto q = p;
                q++;
                person.amo.erase(p);
                p = q;
            }
            else
                p++;
        }
    }
    person.update();
    int x = (person.x_+5) / maze.scalex_;
    int z = (person.z_+5) / maze.scalez_;
    
    Cell * node = maze.Map[x][z];
    for (int i = 0; i < 4; ++i)
    {
        Cell c = Cell::diff(i);
        if (node->neigbhor(i) == NULL)
        {
            Cell next_node = *node + c;
            if (person.collide_wall(Wall(*node, next_node, .2, 3, 1), maze))
            {
                person.move_xz_plane(-1);
            }
        }
    }

    
    glutTimerFunc(1, animate, 1);
    glutPostRedisplay();
}

void specialkeyboard(int key, int x, int y)
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    bool reset = false;
    switch (key)
    {
        case GLUT_KEY_LEFT:
            person.look_xz_plane(0.05); 
            reset = true;
            break;
        case GLUT_KEY_RIGHT: 
            person.look_xz_plane(-0.05); 
            reset = true;
            break;
        case GLUT_KEY_UP:
            xangle -= 5.0;
            if (xangle < 0.0) xangle += 360.0;
            reset = true;
            break;
        case GLUT_KEY_DOWN: 
            xangle += 5.0;
            if (xangle > 360.0) xangle -= 360.0;
            reset = true;
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


int main(int argc, char **argv) 
{
    mygllib::init3d();
    std::cout << "init" << std::endl;
    init();
    
    std::cout << "display" << std::endl;
    glutDisplayFunc(display);
    
    std::cout << "reshape" << std::endl;
    glutReshapeFunc(mygllib::Reshape::reshape);

    
    std::cout << "keyboard" << std::endl;
    glutKeyboardFunc(mygllib::Keyboard::keyboard);

    std::cout << "special keyboard" << std::endl;
    glutSpecialFunc(specialkeyboard);

    std::cout << "animate" << std::endl;
    glutTimerFunc(1, animate, 1);

    
    std::cout << "loop" << std::endl;
    glutMainLoop();
    
    return 0; 
}
