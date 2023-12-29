#include "OGLTextureResourceExtraData.h"
#include "TextureResourceLoader.h"

bool TextureResourceLoader::VLoadResource(
    char* rawBuffer,
    unsigned int rawSize,
    std::shared_ptr<ResHandle> handle)
{
    std::shared_ptr<OGLTextureResourceExtraData> extra =
        std::make_shared<OGLTextureResourceExtraData>();
    
    extra->m_pTexture = new OGLTextureResourceView();
    if (!extra->m_pTexture) {
        printf("TextureResoureLoader OGLTextureResourceView alloc failed\n");
        return false;
    }
    if (!extra->m_pTexture->Load((unsigned char*)rawBuffer, rawSize)) {
        delete extra->m_pTexture;
        return false;
    }
    handle->SetExtra(std::shared_ptr<OGLTextureResourceExtraData>(extra));
    return true;
}

std::shared_ptr<IResourceLoader> CreateJPGResourceLoader()
{
    return std::shared_ptr<IResourceLoader>(new TextureResourceLoader);
}

