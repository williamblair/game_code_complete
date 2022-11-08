#include "SdkMeshResourceLoader.h"
#include "OGLSdkMeshResourceExtraData.h"

bool SdkMeshResourceLoader::VLoadResource(
    char* rawBuffer, unsigned int rawSize,
    std::shared_ptr<ResHandle> handle)
{
    std::shared_ptr<OGLSdkMeshResourceExtraData> extra =
        std::make_shared<OGLSdkMeshResourceExtraData>();
    extra->GetMesh().CreateFromBuffer(
        (uint8_t*)rawBuffer, (size_t)rawSize);
    handle->SetExtra(std::shared_ptr<OGLSdkMeshResourceExtraData>(extra));
    return true;
}
