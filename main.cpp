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
#include "Maze.h"
#include <unordered_set>

mygllib::Light light;
GLfloat light_model_ambient[] = {1.0, 1.0, 1.0, 1.0};
int y_axis_angle = 0;

float wall_height = 3;
int n = 5;
struct WallHash {
    size_t operator()(const Wall& w) const {
        // Combine the 4 integers
        size_t h1 = std::hash<int>()(w.c0.r_);
        size_t h2 = std::hash<int>()(w.c0.c_);
        size_t h3 = std::hash<int>()(w.c1.r_);
        size_t h4 = std::hash<int>()(w.c1.c_);

        return (((h1 * 31) ^ h2) * 31 ^ h3) * 31 ^ h4;
    }
};
std::unordered_set< Wall, WallHash > punched_walls;

void init()
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    view.eyex() = 3.0f;
    view.eyey() = 2.5f;
    view.eyez() = 2.0f;
    view.zNear() = 0.1f;
    view.lookat();    

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);

    srand((unsigned int) time(NULL));

    std::vector<Wall> p_w = build_maze(n, 0, 2);

    print_maze(n, p_w);
    for (int i = 0; i < p_w.size(); ++i)
    {
        if (punched_walls.find(p_w[i]) != punched_walls.end())
        {
            auto p = punched_walls.find(p_w[i]);
            std::cout << *p << ' ' << p_w[i] << ' ' << (*p==p_w[i]) << std::endl;
        }
        punched_walls.insert(p_w[i]);
    }
    std::cout << std::endl;
    for (auto p: punched_walls)
    {
        std::cout << p << std::endl;
    }
    std::cout << "punched size: " << punched_walls.size() << std::endl;
    
    std::cout<< "p_w_size: " << p_w.size() << std::endl;
    
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    mygllib::Light::all_on();
    light.on();
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
}

void draw_wall(float length)
{
    glPushMatrix();
    {
        glScalef(.1, wall_height, length);
        glutSolidCube(1.0f);
    }
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     mygllib::Light::all_off();
    mygllib::draw_axes();
    mygllib::draw_xz_plane();
    mygllib::Light::all_on();

    glColor3f(0.0f, 0.0f, 0.0f);
    // glDisable(GL_LIGHTING);
    // mygllib::draw_axes();
    // mygllib::draw_xz_plane();//-500, 500, -500, 500, 10, 10);

    float color[][3] = {{1, 0, 1}, {0, 1, 0}, {0, 0, 0}, {1, 1, 1}, {0, 0, 1}};
    
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            glPushMatrix();
            {
                
                glColor3fv(color[i]);
                glTranslatef(i, 0, j);
                std::cout << Wall(Cell(i, j-1), Cell(i, j)) << ' ' << Wall(Cell(i-1, j), Cell(i, j)) << std::endl;
                if (punched_walls.find(Wall(Cell(i, j-1), Cell(i, j))) == punched_walls.end()){
                    draw_wall(1); std::cout << "draw ";}
                else
                    std::cout << "not draw ";
                glRotatef(90, 0, 1, 0);
                glTranslatef(0.5, 0, 0.5);
                if (punched_walls.find(Wall(Cell(i-1, j), Cell(i, j))) == punched_walls.end()){
                    draw_wall(1); std::cout << "draw";}
                else
                    std::cout << "not draw";
                std::cout << std::endl;
            }
            glPopMatrix();
        }
        glPushMatrix();
        {
            glTranslatef(i, 0, n);
            glRotatef(90, 0, 1, 0);
            glTranslatef(0.5, 0, 0.5);
            draw_wall(1);
        }
        glPopMatrix();
        glPushMatrix();
        {
            glTranslatef(n, 0, i);
            // glRotatef(90, 0, 1, 0);
            // glTranslatef(0.25, 0, 0.25);
            draw_wall(1);
        }
        glPopMatrix();
    }
    
    std::cout << "\n\n\n\n\n";
    glEnable(GL_LIGHTING);
    
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glEnable(GL_COLOR_MATERIAL);
    // glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    // glFrontFace(GL_CCW);
    glutSwapBuffers();
}


void keyboard(unsigned char key, int x, int y)
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    switch(key)
    {
        case 'x': view.eyex() -= 0.1; break;
        case 'X': view.eyex() += 0.1; break;
        case 'y': view.eyey() -= 0.1; break;
        case 'Y': view.eyey() += 0.1; break;
        case 'z': view.eyez() -= 0.1; break;
        case 'Z': view.eyez() += 0.1; break;
            
        case 'v': view.fovy() -= 0.1; break;
        case 'V': view.fovy() += 0.1; break;            
        case 'a': view.aspect() -= 0.1; break;
        case 'A': view.aspect() += 0.1; break;
        case 'n': view.zNear() -= 0.1; break;
        case 'N': view.zNear() += 0.1; break;
        case 'f': view.zFar() -= 0.1; break;
        case 'F': view.zFar() += 0.1; break;

        
    }
    
    view.set_projection();
    view.lookat();    
    glutPostRedisplay();
}

void specialkeyboard(int key, int x, int y)
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    switch (key)
    {
        case GLUT_KEY_UP:
            view.eyey() += 0.1;
            view.eyex() -= 0.1;
            view.eyez() -= 0.1;
            break;
        case GLUT_KEY_DOWN:
            view.eyey() -= 0.1;
            view.eyex() += 0.1;
            view.eyez() += 0.1;
            break;
        case GLUT_KEY_RIGHT:
            view.eyex() += 0.1;
            view.eyez() -= 0.1;
            break;
        case GLUT_KEY_LEFT:
            view.eyex() -= 0.1;
            view.eyez() += 0.1;
            break;
    }
    view.set_projection();
    view.lookat();    
    glutPostRedisplay();
}

int main(int argc, char ** argv)
{
    mygllib::WIN_W = 700;
    mygllib::WIN_H = 700;
    mygllib::init3d();
    init();
    
    glClearColor(1, 1, 1, 1);
    glClearDepth(1.0f);
    glutDisplayFunc(display);
    glutReshapeFunc(mygllib::Reshape::reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialkeyboard);
    glutMainLoop();
    
    return 0;
}
