#ifndef GCC4_OGL_TEXTURE_RESOURCE_VIEW_H_INCLUDED
#define GCC4_OGL_TEXTURE_RESOURCE_VIEW_H_INCLUDED

//#include <GL/glew.h>
#include <cstdint>

class OGLTextureResourceView
{
public:
    OGLTextureResourceView();
    ~OGLTextureResourceView();

    bool Load(const unsigned char* data, const unsigned int dataLen);

    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }
    int GetBytesPerPixel() const { return m_BytesPerPixel; }
    uint8_t* GetData() const { return m_pData; }

private:
    int m_Width;
    int m_Height;
    int m_BytesPerPixel;
    //GLuint mTexID;
    uint8_t* m_pData;
};

#endif // GCC4_OGL_TEXTURE_RESOURCE_VIEW_H_INCLUDED

