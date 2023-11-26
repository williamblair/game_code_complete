#ifndef GCC4_OGL_TEXTURE_RESOURCE_EXTRA_DATA_H_INCLUDED
#define GCC4_OGL_TEXTURE_RESOURCE_EXTRA_DATA_H_INCLUDED

#include "IResourceExtraData.h"
#include "OGLTextureResourceView.h"

class OGLTextureResourceExtraData : public IResourceExtraData
{
    friend class TextureResourceLoader;

public:
    OGLTextureResourceExtraData() :
        m_pTexture(nullptr)
    {}
    virtual ~OGLTextureResourceExtraData() {
        if (m_pTexture) { delete m_pTexture; }
    }

    virtual std::string VToString() { return "OGLTextureResourceExtraData"; }

    OGLTextureResourceView* GetTexture() { return m_pTexture; }

protected:
    OGLTextureResourceView* m_pTexture;
};

#endif // GCC4_OGL_IRESOURCE_EXTRA_DATA_H_INCLUDED

