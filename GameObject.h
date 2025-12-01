#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Function.h"
#include "myglm.h"

class GameObject
{
public:
    GameObject(float x=0, float y=0, float z=0);
    GameObject(float x, float y, float z,
               float h, float w, float l,
               float r, float g, float b,
               float refx, float refy, float refz);
    GameObject(const glm::vec3 & p0, const glm::vec3 & p1)
        : x_(p0[0]), y_(p0[1]), z_(p0[2]), dir_(p1[0] - p0[0], p1[1] - p0[1], p1[2] - p0[2])
    {
        glm::normalize(dir_);
    }
    void look_xy_plane(float da);
    void look_yz_plane(float da);
    void look_xz_plane(float da);
    void look_y_plane(float da);
    void move_xz_plane(float dp);
    void draw_object(float x=1, float y=1, float z=1) const;
    // void collide(GameObject *) = 0;
    
    float x_, y_, z_, w_, h_, l_, r_, g_, b_;
    glm::vec3 dir_;
};

// class Wall: public GameObject
// {
// public:
//     Wall()
//     {}
    
// };

// class Maze: GameObject
// {};

class Body: public GameObject
{
public:
    Body(float x = 0, float y = 0, float z=0)
        : GameObject(x, y, z), destination_(x, z)
    {}
    Body(const glm::vec3 & position, const glm::vec2 & destination)
        : GameObject(position, glm::vec3(destination[0], position[1], destination[1]))
    {}
    // void move_left()
    // {
    //     look_xz_plane(0.05);
    //     move
    // }
    // void move_right();

    void update_destination(const glm::vec2 & v);
    int turn_to_dest();
    void dir_dest();
    bool automated_movement();
    
    glm::vec2 destination_;
    float da_;
};

class Enemies: public Body
{
public:
    Enemies(int n)
        : Body(rand() % n, 1.5, rand() % n), node_(NULL), state_(0)
    {}
    Enemies(Cell * start_node)
        : Body(start_node->r_, 0, start_node->c_), node_(start_node->rand_neigbhors()),
          state_(0)
    {
        destination_ = glm::vec2(node_->r_, node_->c_);
    }
    void init_node(Cell * node)
    {
        std::cout << "here11 " << node << std::endl;;
        node_ = node->rand_neigbhors();
        
        std::cout << "here12\n";
        destination_ = glm::vec2(node_->r_, node_->c_);
        dir_ = glm::vec3(destination_[0] - x_, 0, destination_[1] - z_);
        glm::normalize(dir_);
    }
    void run()
    {
        std::cout << state_ << std::endl;
        if (state_ == 0)
            state_ = automated_movement();
        else if (state_ == 1)
        {
            glm::vec2 dest(node_->r_, node_->c_);
            update_destination(dest);
            node_ = node_->rand_neigbhors();
            state_ = 2;
        }
        else if (state_ == 2)
        { 
            if (turn_to_dest())
            {
                state_ = 2;
            } 
            else
            {
                dir_dest();
                state_ = 0;
            }
        }
    }
    Cell * node_;
    //=======================================================================
    // state = 0:move, 1:pop_path 2:turn_to_dir, 3:found_fp, 
    //=======================================================================
    int state_;
};

 float rTd = 180 / M_PI;

void draw_cylinder(float radius, float height)
{
    //glPushMatrix();
    GLUquadricObj * p = gluNewQuadric();
    // SECOND RUN
    //gluQuadricDrawStyle(p, GLU_LINE);
    gluQuadricDrawStyle(p, GLU_FILL);
    int slice_per_ring = 20;
    int rings = 20;
    gluCylinder(p, radius, radius, height, slice_per_ring, rings);
    gluDeleteQuadric(p);
    //glPopMatrix();
}


float get_angle(float x1, float x2)
{
    return atan2(x2,x1) * rTd;
}

//=============================================================================
// GameObject:
//=============================================================================
GameObject::GameObject(float x, float y, float z)
    : x_(x), y_(y), z_(z), h_(1), w_(1), l_(1),
      r_(rand() / RAND_MAX), g_(rand() / RAND_MAX), b_(rand() / RAND_MAX), 
      dir_(1 , 0, 0)
{}

GameObject::GameObject(float x, float y, float z,
           float h, float w, float l,
           float r, float g, float b,
           float refx, float refy, float refz)
    : x_(x), y_(y), z_(z), h_(h), w_(w), l_(l),
      r_(r), g_(g), b_(b), 
      dir_((refx - x_), (refy - y), (refz - z_))
{}

void GameObject::look_xy_plane(float da)//float & refx, float & refy, float da)
{
    glm::vec4 d = glm::rotate(da, glm::vec3(0, 0, 1)) * glm::vec4(dir_[0], dir_[1], dir_[2], 1);
    dir_[0] = d[0];
    dir_[1] = d[1];
    dir_[2] = d[2];
        
    dir_ = glm::normalize(dir_);
    // refx = x_ + dir_[0];
    // refy = y_ + dir_[1];
}

void GameObject::look_yz_plane(float da)//float & refy, float & refz, float da)
{
    glm::vec4 d = glm::rotate(da, glm::vec3(1, 0, 0)) * glm::vec4(dir_[0], dir_[1], dir_[2], 1);
    dir_[0] = d[0];
    dir_[1] = d[1];
    dir_[2] = d[2];

    dir_ = glm::normalize(dir_);
    // refy = y_ + dir_[1];
    // refz = z_ + dir_[2];
}

void GameObject::look_xz_plane(float da)//float & refx, float & refz, float da)
{
    // std::cout << "before: " << dir_ << std::endl;
    glm::vec4 d = glm::rotate(da, glm::vec3(0, 1, 0)) * glm::vec4(dir_[0], dir_[1], dir_[2], 1);
    dir_[0] = d[0];
    dir_[1] = d[1];
    dir_[2] = d[2];
    
    // std::cout << "after: " << dir_ << std::endl;
    dir_ = glm::normalize(dir_);
    // std::cout << "after: " << dir_ << std::endl;
    // refx = x_ + dir_[0];
    // refz = y_ + dir_[2]; 
}
void GameObject::look_y_plane(float da)//float & refy, float & refz, float da)
{
    glm::vec4 d = glm::rotate(da, glm::vec3(1, 0, 1)) * glm::vec4(dir_[0], dir_[1], dir_[2], 1);
    dir_[0] = d[0];
    dir_[1] = d[1];
    dir_[2] = d[2];

    dir_ = glm::normalize(dir_);
    // refy = y_ + dir_[1];
    // refz = z_ + dir_[2];
}
void GameObject::move_xz_plane(float dp)
{
    x_ += dp * dir_[0];
    // y_ += dp * dir_[1];
    z_ += dp * dir_[2];
}
void GameObject::draw_object(float scalex, float scaley, float scalez) const
{
    float r = (dir_[0] * dir_[0]) + (dir_[1] * dir_[1]) + (dir_[2] * dir_[2]);
        
    glPushMatrix();
    {
        glTranslatef(x_, y_, z_);
        glScalef(scalex, scaley, scalez);
        // std::cout << "angle: " << get_angle(dir_[0], dir_[2]) << std::endl;
        glRotatef(-get_angle(dir_[0], dir_[2]), 0, 1, 0);
        glPushMatrix();
        {
            glRotatef(90, 0, 1, 0);
            glTranslatef(0, 1, 0);
            draw_cylinder(0.1, 0.5);
        }
        glPopMatrix();
        glScalef(w_, h_, l_);
        glColor3f(r_, g_, b_);
        glutSolidCube(1);

    }
    glPopMatrix();
}

//=============================================================================
// Body:
//=============================================================================
void Body::update_destination(const glm::vec2 & v)
{
    destination_[0] = v[0];
    destination_[1] = v[1];

    glm::vec2 dir_to_dest(destination_[0] - x_, destination_[1] - z_);
    glm::normalize(dir_to_dest);
    // dir_[0] = dist[0];
    // dir_[2] = dist[1];
    // glm::normalize(dir_);
    float theta = get_angle(dir_to_dest[0] - dir_[0], dir_to_dest[1] - dir_[2]);
    da_ = -theta / 1000;
}

int Body::turn_to_dest()
{
    look_xz_plane(da_);
    // std::cout << da_ << std::endl;
    if (da_ < 0)
    {
        return (get_angle(dir_[0], dir_[2]) < get_angle(destination_[0] - x_, destination_[1] - z_));
    }
    return (get_angle(dir_[0], dir_[2]) >= get_angle(destination_[0] - x_, destination_[1] - z_));
}

void Body::dir_dest()
{
    dir_[0] = destination_[0]-x_;
    dir_[2] =  destination_[1]-z_;

    glm::normalize(dir_);
}

bool Body::automated_movement()
{
    //std::cout << x << ' ' << z << std::endl;
    glm::vec2 dist((destination_[0] - x_), (destination_[1] - z_));
     std::cout << dist[0] << ' ' << dist[1] << std::endl;
        
     std::cout << "length: " << glm::length(dist) << ' ' << w_ << std::endl;
    if (glm::length(dist) > 0.1)
    {
         std::cout << x_ << ' ' << y_ << std::endl;
        move_xz_plane(0.05);
         std::cout << x_ << ' ' << y_ << std::endl;
        return 0;
    }
    x_ = destination_[0]; z_ = destination_[1];
    return 1;
}


#endif
