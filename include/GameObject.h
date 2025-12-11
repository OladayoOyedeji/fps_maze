#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Maze.h"

#include "SingletonView.h"

class GameObject
{
public:
    GameObject(float x=0, float y=0, float z=0);
    GameObject(float x, float y, float z,
               float h, float w, float l,
               float r, float g, float b,
               float refx, float refy, float refz);
    GameObject(const glm::vec3 & p0, const glm::vec3 & p1)
        : x_(p0[0]), y_(p0[1]), z_(p0[2]), dir_(p1[0] - p0[0], p1[2] - p0[2])
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
    glm::vec2 dir_;
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
    // void shoot()

    // std::vector < Bullet > amo;
    bool bird_eye_;
    
};

class Enemies: public GameObject
{
public:
    Enemies(Maze * maze)
        : GameObject(rand() % maze->n_, 0, rand() % maze->n_),
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
        std::cout << dist << ' ' << glm::length(dist) << std::endl;
        if (glm::length(dist) > 0.05)
        {
            std::cout << x_ << ' ' << y_ << std::endl;
            std::cout << dir_ << std::endl;
            move_xz_plane(0.05);
            std::cout << x_ << ' ' << y_ << std::endl;
            return 0;
        }
        
        x_ = destination_cell_node_->r_;
        z_ = destination_cell_node_->c_;
        return 1;
    }
    void run()
    {
        

        // 2 states
        // - 0: move to destination
        std::cout << state_ << std::endl;
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
