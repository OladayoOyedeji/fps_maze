// File: GameGlobal.h
#include "global.h"
#include "GameObject.h"
#include <vector>

namespace Global
{   
    extern Maze maze;
    extern First_Person person;
    extern std::vector<Enemies *> enemies;

    inline
        void init( void)
    {
        
        mygllib::View & view = *(mygllib::SingletonView::getInstance());
        
        person.update_eye_ref(view);
        view.zNear() = 0.1;
        view.lookat();
    
        glClearColor(1, 1, 1, 0);
        glClearDepth(1.0f);
        glEnable(GL_DEPTH_TEST);

        mygllib::Light::all_on();
        light.on();

        glGenTextures(3, texture);  // Create texture index array.
        glEnable(GL_TEXTURE_2D);    // Turn on OpenGL 2D texturing.
        
        // Create texture from image file
        // Or "images/launch.bmp"
        Util::load_external_texture(texture[0], "images/rock-taupe.bmp");
        Util::load_external_texture(texture[1], "images/nightSky.bmp");

        for (int i = 0; i < enemies.size(); ++i)
        {
            enemies[i] = new Enemies(&maze);
        }
        //Create texture procedurally
    
        // Specify how texture values combine with current surface
        // color values. In this case, the texture data replaces what's
        // the pixel data.
        //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 
        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
    }
}
