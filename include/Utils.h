#ifndef UTILS_H
#define UTILS_H

#include <GL/freeglut.h>
#include <string>

namespace Util
{
    void build_wall(float scalex=1, float scaley=1, float scalez=1);
    void build_grass(float scalex=1, float scaley=1, float scalez=1);
    void load_external_texture(GLuint texid,
                                      const std::string & filename);
    void load_procedural_textures(GLuint texid, void * const p);
}

#endif
