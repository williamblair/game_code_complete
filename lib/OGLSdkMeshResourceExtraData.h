#ifndef OGL_SDKMESH_RESOURCE_EXTRADATA_H_INCLUDED
#define OGL_SDKMESH_RESOURCE_EXTRADATA_H_INCLUDED

#include "IResourceExtraData.h"

// https://github.com/StanwieCB/sdkmesh-to-obj
#include "sdkmesh.h"

class OGLSdkMeshResourceExtraData : public IResourceExtraData
{
    friend class SdkMeshResourceLoader;
public:
    OGLSdkMeshResourceExtraData() {}
    virtual ~OGLSdkMeshResourceExtraData() {}
    virtual std::string VToString() { return "OGLSdkMeshResourceExtraData"; }
    
    SdkMesh& GetMesh() { return m_Mesh; }
private:
    SdkMesh m_Mesh;
};

#endif // OGL_SDKMESH_RESOURCE_EXTRADATA_H_INCLUDED
