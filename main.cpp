#include "Function.h"
#include "GameObject.h"
 
mygllib::Light light;
GLfloat light_model_ambient[] = {1.0, 1.0, 1.0, 1.0};
int y_axis_angle = 0;

float wall_height = 3;
const int n = 20;
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
std::stack< glm::vec2 > path;
Cell * Map[n][n];

GameObject person(0, 1.5, 0, 1, 1, 1, 0, 0, 0, 1, 0, 1);
Body body(0, 1.5, 0);
Body body1(10, 1.5, 5);
Enemies enemy(n);

void init()
{
    mygllib::View & view = *(mygllib::SingletonView::getInstance());
    view.eye(person.x_, person.y_ + 10, person.z_);
    float refx = person.dir_[0] + person.x_;
    float refy = person.dir_[1] + person.y_;
    float refz = person.dir_[2] + person.z_;
    view.ref(refx, refy, refz);
    view.zNear() = 0.1f;
    view.lookat();    

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearDepth(1.0f);

    srand((unsigned int) time(NULL));

    std::vector<Wall> p_w = build_maze(n, 0, 2);

    for (int i = 0; i < n; ++i )
    {
        for (int j = 0; j < n; j++)
        {
            Map[i][j] = new Cell(i, j);
        } 
    }
    
    for (int i = 0; i < p_w.size(); ++i)
    {
        //  std::cout << "start? " << i  << std::endl;
        punched_walls.insert(p_w[i]);
        // std::cout << "mid? "  << std::endl;
        // std::cout << p_w[i] << std::endl;
        Wall w = Wall(p_w[i].c1, p_w[i].c0);
        // std::cout << w << std::endl;
        punched_walls.insert(w);
        // std::cout << "end? "  << std::endl;
        Map[p_w[i].c1.r_][p_w[i].c1.c_]->open_neigbhors_.push_back(Map[p_w[i].c0.r_][p_w[i].c0.c_]);
        Map[p_w[i].c0.r_][p_w[i].c0.c_]->open_neigbhors_.push_back(Map[p_w[i].c1.r_][p_w[i].c1.c_]);
    }
    int r = enemy.x_;
    int c = enemy.z_;
    // std::cout << "hello\n";
    enemy.init_node(Map[r][c]);
    // std::cout << "not hello\n";
    // for (int i = 0; i < n; ++i)
    // {
    //     for (int j = 0; j < n; ++j)
    //     {
    //         Cell * p = Map[i][j]; 
    //         std::cout << *(Map[i][j]) << "->";
    //         std::string delim = "(";
    //         for (int k = 0; k < n; ++k)
    //         {
    //             std::cout << delim << p->open_neigbhors_[k]; 
    //             delim = ", "; 
    //         }
    //         std::cout << ")\n"
    //     } 
    // };;
     
    print_maze(n, p_w);
    path.push(glm::vec2(9, 4));
    path.push(glm::vec2(6, 4));
    path.push(glm::vec2(6, 3));
    path.push(glm::vec2(2, 3));
    // std::cout << "where do you wan him to go: ";
    // std::cin >> x_des >> y_des;
    
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
 
    glColor3f(1.0f, 0.0f, 0.0f);
    // glDisable(GL_LIGHTING);
    // mygllib::draw_axes();
    // mygllib::draw_xz_plane();//-500, 500, -500, 500, 10, 10);
 
    
     
    //float color[][3] = {{1, 0, 1}, {0, 1, 0}, {0, 0, 0}, {1, 1, 1}, {0, 0, 1}};
    
    person.draw_object();
    // body.draw_object();
    // body1.draw_object();
    glPushMatrix();
    glTranslatef(-10, 1.5, -10);
    glScalef(5, 1, 5);
    enemy.draw_object(float(1)/5, 1, float(1)/5);
    
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            glPushMatrix(); 
            {
                
               glTranslatef(i, 0, j);

               // glutSolidSphere(0.5, 20, 20);
               glTranslatef(0.5, 0, 0);
               
               
               if (punched_walls.find(Wall(Cell(i, j), Cell(i+1, j))) == punched_walls.end())
               {
                    draw_wall(1); 
               }
               
               glRotatef(90, 0, 1, 0);
               glTranslatef(-0.5, 0, -0.5);
               if (punched_walls.find(Wall(Cell(i, j), Cell(i, j+1))) == punched_walls.end())
               {
                   draw_wall(1); 
               }  
            }
            glPopMatrix();
        }
        // glPushMatrix();
        // {
        //     glTranslatef(i, 0, n); 
        //     glRotatef(90, 0, 1, 0);
        //     glTranslatef(0.5, 0, 0);
        //     draw_wall(1);
        // }
        // glPopMatrix();
        // glPushMatrix();
        // {
        //     glTranslatef(n, 0, i);
        //     // glRotatef(90, 0, 1, 0);
        //     // glTranslatef(0.25, 0, 0.25);
        //     glTranslatef(-0.5, 0, 0);
        //     draw_wall(1);
        // }
        // glPopMatrix();
    }
    glPopMatrix();
    // std::cout << "\n\n\n\n\n";
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
    // 0:move, 1:pop_path 2:turn_to_dir,
    static int state = 1;
    
    // switch(key)
    // {
    //     // case 'x': view.eyex() -= 0.1; break;
    //     // case 'X': view.eyex() += 0.1; break;
    //     // case 'y': view.eyey() -= 0.1; break;
    //     // case 'Y': view.eyey() += 0.1; break;
    //     // case 'z': view.eyez() -= 0.1; break;
    //     // case 'Z': view.eyez() += 0.1; break;
            
    //     // case 'v': view.fovy() -= 0.1; break;
    //     // case 'V': view.fovy() += 0.1; break;            
    //     // case 'a': view.aspect() -= 0.1; break;
    //     // case 'A': view.aspect() += 0.1; break;
    //     // case 'n': view.zNear() -= 0.1; break;
    //     // case 'N': view.zNear() += 0.1; break;
    //     // case 'f': view.zFar() -= 0.1; break;
    //     // case 'F': view.zFar() += 0.1; break;
    //     case 'a': person.look_xz_plane(0.05); break;
    //     case 'w': person.look_y_plane(0.1); break;
    //     case 's': person.look_y_plane(-0.1); break;
    //     case 'd': person.look_xz_plane(-0.05); break;
    //     case 'i': person.move_xz_plane(-0.5); break;
    //     case 'k': person.move_xz_plane(0.5); break;
    //     case 'u':
    //         enemy.run();
    //         // glm::vec2 dest(10, 5);
    //         // body.update_destination(dest);
    //         // std::cout << "dest: " << body.turn_to_dest() << std::endl;
    //         break; 
    // }

    if (key == 'a')
    {person.look_xz_plane(0.05);}
    if (key == 'w')
    {}
    if (key == 's')
    {}
    if (key == 'd')
    {person.look_xz_plane(-0.05);}
    if (key == 'i')
    {
        person.move_xz_plane(-0.5);
    }
    if (key == 'k')
    {
        person.move_xz_plane(0.5);
    }
    if (key == 'u')
    {
        enemy.run();
    }
    
    view.eye(person.x_, person.y_+10, person.z_);
    float refx = person.dir_[0] + person.x_;
    float refy = person.dir_[1] + person.y_;
    float refz = person.dir_[2] + person.z_;
    // std::cout << person.dir_[0] << ' ' << person.dir_[1] << ' ' << person.dir_[2] << ' ' << person.z_ << std::endl;
    view.ref(refx, refy, refz);
     
    // std::cout << "eye: " << view.eyex() << ' ' << view.eyey() << ' ' << view.eyez() << std::endl;
    // std::cout << "ref: " << view.refx() << ' ' << view.refy() << ' ' << view.refz() << std::endl;
    // std::cout << view.eyex() << ' ' << view.eyey() << ' ' << view.eyez() << std::endl;
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
            person.look_xz_plane(0.05); break;
        case GLUT_KEY_LEFT:
            person.look_xz_plane(-0.05); break;
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
