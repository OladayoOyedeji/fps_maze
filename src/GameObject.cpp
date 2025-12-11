#include "GameObject.h"

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
      dir_(1 , 0)
{}

GameObject::GameObject(float x, float y, float z,
           float h, float w, float l,
           float r, float g, float b,
           float refx, float refy, float refz)
    : x_(x), y_(y), z_(z), h_(h), w_(w), l_(l),
      r_(r), g_(g), b_(b), 
      dir_((refx - x_), (refz - z_))
{}

void GameObject::look_xy_plane(float da)//float & refx, float & refy, float da)
{
    glm::vec4 d = glm::rotate(da, glm::vec3(0, 0, 1)) * glm::vec4(dir_[0], 0, dir_[1], 1);
    dir_[0] = d[0];
    
    dir_[1] = d[2];
        
    dir_ = glm::normalize(dir_);
    // refx = x_ + dir_[0];
    // refy = y_ + dir_[1];
}

void GameObject::look_yz_plane(float da)//float & refy, float & refz, float da)
{
    glm::vec4 d = glm::rotate(da, glm::vec3(1, 0, 0)) * glm::vec4(dir_[0], 0, dir_[1], 1);
    dir_[0] = d[0];
    
    dir_[1] = d[2];

    dir_ = glm::normalize(dir_);
    // refy = y_ + dir_[1];
    // refz = z_ + dir_[2];
}

void GameObject::look_xz_plane(float da)//float & refx, float & refz, float da)
{
    // std::cout << "before: " << dir_ << std::endl;
    glm::vec4 d = glm::rotate(da, glm::vec3(0, 1, 0)) * glm::vec4(dir_[0], 0, dir_[1], 1);
    dir_[0] = d[0];
    
    dir_[1] = d[2];
    
    // std::cout << "after: " << dir_ << std::endl;
    dir_ = glm::normalize(dir_);
    // std::cout << "after: " << dir_ << std::endl;
    // refx = x_ + dir_[0];
    // refz = y_ + dir_[2]; 
}
void GameObject::look_y_plane(float da)//float & refy, float & refz, float da)
{
    glm::vec4 d = glm::rotate(da, glm::vec3(1, 0, 1)) * glm::vec4(dir_[0], 0, dir_[1], 1);
    dir_[0] = d[0];
    
    dir_[1] = d[2];

    dir_ = glm::normalize(dir_);
    // refy = y_ + dir_[1];
    // refz = z_ + dir_[2];
}
void GameObject::move_xz_plane(float dp)
{
    x_ += dp * dir_[0];
    // y_ += dp * dir_[1];
    z_ += dp * dir_[1];
}
void GameObject::draw_object(float scalex, float scaley, float scalez) const
{
    //float r = (dir_[0] * dir_[0]) + (dir_[1] * dir_[1]) + (dir_[2] * dir_[2]);
        
    glPushMatrix();
    {
        glTranslatef(x_, y_, z_);
        glRotatef(-get_angle(dir_[0], dir_[1]), 0, 1, 0);
        
        glScalef(scalex, scaley, scalez);
        // std::cout << x_ << ' ' << z_ << std::endl;
        // std::cout << "angle: " << get_angle(dir_[0], dir_[2]) << std::endl;
        
        glPushMatrix();
        {
            glRotatef(90, 0, 1, 0);
            glTranslatef(0, 1, 0);
            draw_cylinder(0.1, 0.5);
        }
        glPopMatrix();
        glScalef(w_, h_, l_);
        glTranslatef(-w_/2, 0, 0);
        glColor3f(r_, g_, b_);
        glutSolidCube(1);

    }
    glPopMatrix();
}

void Enemies::draw_enemy()
{
    glPushMatrix();
    {
        //glScalef(position_scale[0], 1, position_scale[1]);
        glTranslatef(x_ * position_scale[0], y_, z_ * position_scale[1]);
        
        glRotatef(-get_angle(dir_[0], dir_[1]), 0, 1, 0);
            
            
        // std::cout << x_ << ' ' << z_ << std::endl;
        // std::cout << "angle: " << get_angle(dir_[0], dir_[2]) << std::endl;
        
        glPushMatrix();
        {
            glRotatef(90, 0, 1, 0);
            glTranslatef(0, 1, 0);
            draw_cylinder(0.1, 0.5);
        }
        glPopMatrix();
        glScalef(w_, h_, l_);
        glTranslatef(-w_/2, 0, 0);
        glColor3f(r_, g_, b_);
        glutSolidCube(1);
    }
    glPopMatrix();
}

void First_Person::update_eye_ref(mygllib::View & view)
{
    float bird_eyex, bird_eyey, bird_eyez;
    float bird_refx, bird_refy, bird_refz;
    
    if (bird_eye_)
    {
        
        bird_eyex = 1;
        bird_eyey = 30;
        bird_eyez = 1;
        bird_refx = 1;
        bird_refy = 0;
        bird_refz = 0;
    }
    else
    {
        bird_eyex = 0;
        bird_eyey = 0;
        bird_eyez = 0;
        bird_refx = 1;
        bird_refy = 0;
        bird_refz = 1;
    }
    
    view.eye(x_, bird_eyey + y_, z_);
    float refx = dir_[0] * bird_refx + x_;
    float refy = y_;
    float refz = dir_[1] * bird_refz + z_;
    // std::cout << dir_[0] << ' ' << dir_[1] << ' ' << dir_[2] << ' ' << z_ << std::endl;
    view.ref(refx, refy, refz);
}

//=============================================================================
// Body:
//=============================================================================
// void Body::update_destination(const glm::vec2 & v)
// {
//     destination_[0] = v[0];
//     destination_[1] = v[1];

//     // glm::vec2 dir_to_dest(x_ - destination_[0], z_ - destination_[1]);
//     // glm::normalize(dir_to_dest);
//     // // dir_[0] = dist[0];
//     // // dir_[2] = dist[1];
//     // // glm::normalize(dir_);
//     // float theta = get_angle(dir_to_dest[0] - dir_[0], dir_to_dest[1] - dir_[2]);
//     // da_ = theta / 1000;
// }

// int Body::turn_to_dest()
// {
//     // look_xz_plane(da_);
//     // // std::cout << da_ << std::endl;
//     // if (da_ < 0)
//     // {
//     //     return (get_angle(dir_[0], dir_[2]) < get_angle(destination_[0] - x_, destination_[1] - z_));
//     // }
//     dir_ = glm::vec3(destination_[0] - x_, 0, destination_[1] - z_);
//     return 0;
// }

// void Body::dir_dest()
// {
//     // dir_[0] = destination_[0]-x_;
//     // dir_[2] = destination_[1]-z_;

//     glm::normalize(dir_);
// }

// bool Body::automated_movement()
// {
//     //std::cout << x << ' ' << z << std::endl;
//     glm::vec2 dist((destination_[0] - x_), (destination_[1] - z_));
//      // std::cout << dist[0] << ' ' << dist[1] << std::endl;
        
//      // std::cout << "length: " << glm::length(dist) << ' ' << w_ << std::endl;
//     if (glm::length(dist) > 0.1)
//     {
//         // std::cout << x_ << ' ' << y_ << std::endl;
//         move_xz_plane(0.05);
//         // std::cout << x_ << ' ' << y_ << std::endl;
//         return 0;
//     }
//     x_ = destination_[0]; z_ = destination_[1];
//     return 1;
// }
