#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Maze.h"
#include <list>
#include "SingletonView.h"

inline
glm::vec2 perp(const glm::vec2& v) {
    return glm::vec2(-v.y, v.x);
}

inline
void project(const std::vector<glm::vec2>& poly, const glm::vec2& axis,
             float &min, float &max)
{
    float first = glm::dot(poly[0], axis);
    min = max = first;

    for (int i = 1; i < poly.size(); i++) {
        float p = glm::dot(poly[i], axis);
        if (p < min) min = p;
        if (p > max) max = p;
    }
}

inline
bool overlap(float minA, float maxA, float minB, float maxB) {
    return !(maxA < minB || maxB < minA);
}

inline
bool collide(const std::vector<glm::vec2>& A, const std::vector<glm::vec2>& B) {
    std::vector<glm::vec2> axes;

    auto addAxes = [&](const std::vector<glm::vec2>& P) {
        for (int i = 0; i < 4; i++) {
            glm::vec2 edge = P[(i+1)%4] - P[i];
            axes.push_back(perp(edge));
        }
    };

    addAxes(A);
    addAxes(B);

    for (auto& axis : axes) {
        float minA, maxA, minB, maxB;
        project(A, axis, minA, maxA);
        project(B, axis, minB, maxB);

        if (!overlap(minA, maxA, minB, maxB))
            return false;
    }

    return true;
}



class GameObject
{
public:
    GameObject(float x=0, float y=0, float z=0);
    GameObject(float x, float y, float z, glm::vec2 & dir)
        : x_(x), y_(y), z_(z), h_(1), w_(1), l_(1),
          r_(rand() / RAND_MAX), g_(rand() / RAND_MAX), b_(rand() / RAND_MAX), 
          dir_(dir)
    {
        // std::cout << x << ' ' << y << ' ' << z << std::endl;
    }
    GameObject(float x, float y, float z,
               float h, float w, float l,
               float r, float g, float b,
               float refx, float refy, float refz);
    GameObject(const glm::vec3 & p0, const glm::vec3 & p1)
        : x_(p0[0]), y_(p0[1]), z_(p0[2]), dir_(p1[0] - p0[0], p1[2] - p0[2])
    {
        glm::normalize(dir_);
    }
    bool collide_wall(Wall wall, const Maze & m)
    {
        std::vector< glm::vec2 > A;
        float x = (wall.x() - (wall.w_ / 2)) * m.scalex_;
        float z = (wall.z() - (wall.l_ / 2)) * m.scalez_;

        A.push_back(glm::vec2(x, z));

        x = x + wall.w_ * m.scalex_;
        
        A.push_back(glm::vec2(x, z));

        z = z + wall.l_ * m.scalez_;

        A.push_back(glm::vec2(x, z));

        x = x - wall.w_ * m.scalex_;
        
        A.push_back(glm::vec2(x, z));

        std::vector<glm::vec2> B;
        x = (x_ - 0.5);
        z = (z_ - 0.5);

        B.push_back(glm::vec2(x, z));

        x = x + 1;
        
        B.push_back(glm::vec2(x, z));

        z = z + 1;

        B.push_back(glm::vec2(x, z));

        x = x - 1;
        
        B.push_back(glm::vec2(x, z));

        return collide(A, B);
    }
    
    void look_xy_plane(float da);
    void look_yz_plane(float da);
    void look_xz_plane(float da);
    void look_y_plane(float da);
    void move_xz_plane(float dp);
    virtual void draw_object(float x=1, float y=1, float z=1);
    // void collide(GameObject *) = 0;
    
    float x_, y_, z_, w_, h_, l_, r_, g_, b_;
    glm::vec2 dir_;
};

class Bullet: public GameObject
{
public:
    Bullet(float x, float y, float z, glm::vec2 & dir)
        : GameObject(x, y, z, dir)
    {
        
    }
     void draw_object(float x=1, float y=1, float z=1);
};

class First_Person: public GameObject
{
public:
    First_Person(float x=0, float z=0)
        : GameObject(x, 1.5, z), bird_eye_(false)
    {}
    
    void update_eye_ref(mygllib::View & view);
    
    void change_view()
    {
        bird_eye_ = !bird_eye_;
    }
    void update()
    {
        //std::cout << "amooo\n";
        
        for (auto p: amo)
        {
            //std::cout << p.dir_ << ' ' << p.x_ << ' ' << p.z_ << std::endl;
            p->move_xz_plane(0.5);
        }
    }
     void draw_object(float scalex=1, float scaley=1, float scalez=1);
    void shoot()
    {
        amo.push_back(new Bullet(x_, y_, z_, dir_));
    }

    std::list < Bullet * > amo;
    bool bird_eye_;
    
};

class Enemies: public GameObject
{
public:
    Enemies(Maze * maze)
        : GameObject(rand() % maze->n_, 1.5, rand() % maze->n_),
          destination_cell_node_(maze->Map[int(x_)][int(z_)]),
          position_scale(maze->scalex_, maze->scalez_), state_(0)
    {
        destination_cell_node_ = destination_cell_node_->rand_neigbhors(1, 0);
        Cell c = Cell(destination_cell_node_->r_, destination_cell_node_->c_) + -Cell(x_, z_);
        dir_[0] = c.r_;
        dir_[1] = c.c_;
    }
    void draw_enemy();
    int move_to_destination()
    {
        glm::vec2 dist((destination_cell_node_->r_ - x_), (destination_cell_node_->c_ - z_));
        
        if (glm::length(dist) > 0.01)
        {
            move_xz_plane(0.01);
            return 0;
        }
        
        x_ = destination_cell_node_->r_;
        z_ = destination_cell_node_->c_;
        return 1;
    }
    bool collide_object(const GameObject * object)
    {
        float x = x_ * position_scale[0];
        float z = z_ * position_scale[1];
        glm::vec2 dist(object->x_ - x, object->z_ - z);
        std::cout << glm::length(dist) << std::endl;
        //std::cout << x_ << ' ' << y_ << std::endl;
        return (glm::length(dist) <= 1.3);
    }
    void run()
    {
        if (state_ == -1) return;
        if (state_ == 0)
        {
            state_ = move_to_destination();
        }
        else
        {
            destination_cell_node_ = destination_cell_node_->rand_neigbhors(dir_[0], dir_[1]);
            Cell c = Cell(destination_cell_node_->r_, destination_cell_node_->c_) + -Cell(x_, z_);
            dir_[0] = c.r_;
            dir_[1] = c.c_;
            state_ = 0;
        }
        // - 1: update new destination
    }
    
    Cell * destination_cell_node_;
    glm::vec2 position_scale;
    // state -1
    int state_;
    
};

// class Enemies: public Body
// {
// public:
//     Enemies(int n)
//         : Body(rand() % n, 1.5, rand() % n), node_(NULL), state_(0)
//     {}
//     Enemies(Cell * start_node)
//         : Body(start_node->r_, 0, start_node->c_), node_(start_node),
//           state_(0)
//     {
//         destination_ = glm::vec2(node_->r_, node_->c_);
//     }
//     void init_node(Cell * node)
//     {
//         // std::cout << "here11 " << node << std::endl;;
//         node_ = node->rand_neigbhors(dir_[0], dir_[2]);
        
//         //  std::cout << "here12\n";
//         destination_ = glm::vec2(node_->r_, node_->c_);
//         dir_ = glm::vec3(destination_[0] - x_, 0, destination_[1] - z_);
//         glm::normalize(dir_);
//     }
//     void run()
//     {
//         // std::cout << "State: " << state_ << std::endl;
//         Cell c = Cell(x_, y_) + -Cell(node_->r_, node_->c_);
        
//         x_ = node_->r_;
//         y_ = node_->c_;

//         node_ = node_->rand_neigbhors(c.r_, c.c_);
//     }
//     Cell * node_;
//     //=======================================================================
//     // state = 0:move, 1:pop_path 2:turn_to_dir, 3:found_fp, 
//     //=======================================================================
//     int state_;
// };

#endif
