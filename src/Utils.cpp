#include "Utils.h"
#include "global.h"
#include "Material.h"
#include "Light.h"

#include "ImageFile.h"

using namespace mygllib;
using namespace Global;

namespace Util
{
    void build_grass(float scalex, float scaley, float scalez)
    {
        glEnable(GL_TEXTURE_2D);
        
        //build_cube();
        // glRotatef(xangle, 1.0, 0.0, 0.0); 
        // glRotatef(yangle, 0.0, 1.0, 0.0); 
        glScalef(scalex, scaley, scalez);
        //glutSolidCube(1);
        // Select texture with id texture[id]
        glBindTexture(GL_TEXTURE_2D, texture[2]);

        // Choose GL_REPLACE or GL_MODULATE
        if (env == 0)
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        else
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
        glBegin(GL_POLYGON);
        {
            //glColor3f(0, 1, 0); //??
            glTexCoord2f(0.0, 0.0); glVertex3f( 0.5, -0.5,  0.5);
            glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -0.5,  0.5);
            glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
            glTexCoord2f(0.0, 1.0); glVertex3f( 0.5, -0.5, -0.5);
        }
        glEnd();
    }
    void build_wall(float scalex, float scaley, float scalez)
    {
        glEnable(GL_TEXTURE_2D);
        
        //build_cube();
        // glRotatef(xangle, 1.0, 0.0, 0.0); 
        // glRotatef(yangle, 0.0, 1.0, 0.0); 
        glScalef(scalex, scaley, scalez);
        // Select texture with id texture[id]
        glBindTexture(GL_TEXTURE_2D, texture[id]);

        // Choose GL_REPLACE or GL_MODULATE
        if (env == 0)
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        else
            glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
        glBegin(GL_POLYGON);
        {
            //glColor3f(0, 1, 0); //??
            glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, 0.5);
            glTexCoord2f(1.0, 0.0); glVertex3f( 0.5, -0.5, 0.5);
            glTexCoord2f(1.0, 1.0); glVertex3f( 0.5,  0.5, 0.5);
            glTexCoord2f(0.0, 1.0); glVertex3f(-0.5,  0.5, 0.5);
        }
        glEnd();
        glBegin(GL_POLYGON);
        {
            //glColor3f(0, 1, 0); //??
            glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
            glTexCoord2f(1.0, 0.0); glVertex3f( 0.5, -0.5, -0.5);
            glTexCoord2f(1.0, 1.0); glVertex3f( 0.5,  0.5, -0.5);
            glTexCoord2f(0.0, 1.0); glVertex3f(-0.5,  0.5, -0.5);
        }
        glEnd();
        glBegin(GL_POLYGON);
        {
            //glColor3f(0, 1, 0); //??
            glTexCoord2f(0.0, 0.0); glVertex3f( 0.5, 0.5,  0.5);
            glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, 0.5,  0.5);
            glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, 0.5, -0.5);
            glTexCoord2f(0.0, 1.0); glVertex3f( 0.5, 0.5, -0.5);
        }
        glEnd();
        glBegin(GL_POLYGON);
        {
            //glColor3f(0, 1, 0); //??
            glTexCoord2f(0.0, 0.0); glVertex3f( 0.5, -0.5,  0.5);
            glTexCoord2f(1.0, 0.0); glVertex3f(-0.5, -0.5,  0.5);
            glTexCoord2f(1.0, 1.0); glVertex3f(-0.5, -0.5, -0.5);
            glTexCoord2f(0.0, 1.0); glVertex3f( 0.5, -0.5, -0.5);
        }
        glEnd();
        
        glBegin(GL_POLYGON);
        {
            //glColor3f(0, 1, 0); //??
            glTexCoord2f(0.0, 0.0); glVertex3f( 0.5, -0.5, -0.5);
            glTexCoord2f(1.0, 0.0); glVertex3f( 0.5,  0.5, -0.5);
            glTexCoord2f(1.0, 1.0); glVertex3f( 0.5,  0.5,  0.5);
            glTexCoord2f(0.0, 1.0); glVertex3f( 0.5, -0.5,  0.5);
        }
        glEnd();
        glBegin(GL_POLYGON);
        {
            //glColor3f(0, 1, 0); //??
            glTexCoord2f(0.0, 0.0); glVertex3f(-0.5, -0.5, -0.5);
            glTexCoord2f(1.0, 0.0); glVertex3f(-0.5,  0.5, -0.5);
            glTexCoord2f(1.0, 1.0); glVertex3f(-0.5,  0.5,  0.5);
            glTexCoord2f(0.0, 1.0); glVertex3f(-0.5, -0.5,  0.5);
        }
        glEnd();
    }

    void load_external_texture(GLuint texid,
                               const std::string & filename)
    {
        // Select texture id texture[0]
        glBindTexture(GL_TEXTURE_2D, texid); 
    
        // Set texture parameters for wrapping
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
        // Set texture parameters for filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    
        // Load image data into texture
        BMPFile image(filename);
        glTexImage2D(GL_TEXTURE_2D,
                     0,                // level of detail
                     GL_RGB,           // texture image format
                     image.width(),    // image width
                     image.height(),   // image height
                     0,              
                     GL_RGB,           // image format (same as
                     // texture image format)
                     GL_UNSIGNED_BYTE, // type for image data
                     image.data());    // address of image
    }

// Routine to losad a program-generated image as a texture.
    void load_procedural_textures(GLuint texid, void * const p)
    {
        // Select texture id texture[1]. 
        glBindTexture(GL_TEXTURE_2D, texid); 

        // Set texture parameters for wrapping.    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Set texture parameters for filtering
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        // Load image data into texture
        glTexImage2D(GL_TEXTURE_2D,
                     0, GL_RGB, 64, 64, 0,
                     GL_RGB, GL_UNSIGNED_BYTE, (GLubyte *) p);
    }
}
