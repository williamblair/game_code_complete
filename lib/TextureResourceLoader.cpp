#include "TextureResourceLoader.h"

bool TextureResourceLoader::VLoadResource(
        char* rawBuffer,
        unsigned int rawSize,
        std::shared_ptr<ResHandle> handle)
{
    std::shared_ptr<OGLTextureResourceExtraData> extra =
        std::make_shared<OGLTextureResourceExtraData>();
    

}
