#ifndef GCC4_OGL_TEXTURE_RESOURCE_VIEW_H_INCLUDED
#define GCC4_OGL_TEXTURE_RESOURCE_VIEW_H_INCLUDED

#include <GL/glew.h>

class OGLTextureResourceView
{
public:
    OGLTextureResourceView();
    ~OGLTextureResourceView();

    bool Load(const unsigned char* data, const unsigned int dataLen);

private:
    int mWidth;
    int mHeight;
    int mBytesPerPixel;
    GLuint mTexID;
};

#endif // GCC4_OGL_TEXTURE_RESOURCE_VIEW_H_INCLUDED

