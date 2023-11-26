#include <cstdio>
#include <cstring>

#include "OGLTextureResourceView.h"
#include "stb_image.h"

OGLTextureResourceView::OGLTextureResourceView() :
    m_Width(0),
    m_Height(0),
    m_BytesPerPixel(0),
    m_pData(nullptr)
{}

OGLTextureResourceView::~OGLTextureResourceView()
{
    //if (mTexID) { glDeleteTextures(1,&mTexID); }
    if (m_pData) { delete[] m_pData; }
}

bool OGLTextureResourceView::Load(const unsigned char* data, const unsigned int dataLen)
{
    //stbi_set_flip_vertically_on_load(true);
    unsigned char* texData =
        stbi_load_from_memory(data,dataLen, &m_Width,&m_Height,&m_BytesPerPixel, 0);
    if (!texData) {
        printf("OGLTextureResourceView failed to load data\n");
        return false;
    }
#if 0
    glGenTextures(1, &mTexID);
    glBindTexture(GL_TEXTURE_2D, mTexID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(
        GL_TEXTURE_2D, // target
        0, // level
        (mBytesPerPixel == 3) ? GL_RGB8 : GL_RGBA8, // internal format
        mWidth, // width
        mHeight, // height
        0, // border
        (mBytesPerPixel == 3) ? GL_RGB : GL_RGBA, // format
        GL_UNSIGNED_BYTE, // type
        texData // data
    );
#endif
    m_pData = new uint8_t[m_Width*m_Height*m_BytesPerPixel];
    if (!m_pData) {
        printf("OGLTextureResourceView failed to alloc pixel data buf\n");
        return false;
    }
    memcpy(m_pData, texData, m_Width*m_Height*m_BytesPerPixel);
    stbi_image_free(texData);
    return true;
}

