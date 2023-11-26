#ifndef GCC4_OGL_CREATE_TEXTURE_H_INCLUDED
#define GCC4_OGL_CREATE_TEXTURE_H_INCLUDED

#include <cstdint>
#include <GL/glew.h>

static inline unsigned int
OGLCreateTexture(uint8_t* data, int width, int height, int bytesPerPixel)
{
    unsigned int texId;
    glGenTextures(1, &texId);
    glBindTexture(GL_TEXTURE_2D, texId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(
        GL_TEXTURE_2D, // target
        0, // level
        (bytesPerPixel == 3) ? GL_RGB8 : GL_RGBA8, // internal format
        width, // width
        height, // height
        0, // border
        (bytesPerPixel == 3) ? GL_RGB : GL_RGBA, // format
        GL_UNSIGNED_BYTE, // type
        data // data
    );
    return texId;
}

#endif // GCC4_OGL_CREATE_TEXTURE_H_INCLUDED
