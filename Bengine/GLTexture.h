#ifndef GLTEXTURE_H_INCLUDED
#define GLTEXTURE_H_INCLUDED

#include <GLEW/glew.h>
#include <string>

namespace Bengine{
    struct GLTexture{
        std::string filePath = "";
        GLuint id = 0;
        int height = 0;
        int width = 0;
    };
}

#endif // GLTEXTURE_H_INCLUDED
