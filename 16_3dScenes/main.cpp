#include <cstdio>
#include <memory>
#include <stdexcept>

#include <SDL2/SDL.h>

#include <GCC4/GCCMath.h>
#include <GCC4/IRenderer.h>
#include <GCC4/Scene.h>
#include <GCC4/ResCache.h>
#include <GCC4/ResourceZipFile.h>
#include <GCC4/AssimpMeshResourceLoader.h>
#include <GCC4/TextureResourceLoader.h>
#include <GCC4/AssimpMeshNode.h>
#include <GCC4/LightNode.h>
#include <GCC4/OGLRenderer.h>
#include <GCC4/OGLVertexBuffer.h>
#include <GCC4/OGLSkyNode.h>
#include <GCC4/GameCodeApp.h>

std::shared_ptr<IRenderer>
createRenderer()
{
    std::shared_ptr<OGLRenderer> rndr =
        std::make_shared<OGLRenderer>();
    if (!rndr->Init(640,480)) {
        throw std::runtime_error("Failed to init renderer");
    }
    return rndr;
}

std::shared_ptr<LightNode>
createLight()
{
    LightNode::LightProperties props;
    Color diffuseCol;
    Mat4x4 tform;
    memset(&props, 0, sizeof(props));
    diffuseCol.Set(1.0f,0.0f,1.0f,1.0f);
    tform = Translate(0.0f,1.0f,0.0f) * Rotate(Deg2Rad(90.0f),Vec3(0.0f,1.0f,0.0f));
    std::shared_ptr<LightNode> node = std::make_shared<LightNode>(ActorId(2), "Test Light", props, diffuseCol, &tform);
    return node;
}

bool updateInput()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            return true;
            break;
        default:
            break;
        }
    }
    return false;
}

class TestGCCApp : public GameCodeApp
{
public:
    virtual const char* VGetGameTitle() { return "TestGCCApp"; }
    virtual const char* VGetGameAppDirectory() { return "./"; }
};
TestGCCApp g_TestGCCApp;

// Required globals - TODO move
ResCache* g_ResCache = nullptr;

GameCodeApp* g_pApp = &g_TestGCCApp;

#ifdef _WIN32
#undef main
#endif

int main(int argc, char **argv)
{
    Mat4x4 modelMat = Translate(0.0f,-2.0f,-9.0f)*Scale(0.05f,0.05f,0.05f);
    Mat4x4 viewMat = Mat4x4::g_Identity;
    Mat4x4 projMat = Perspective(Deg2Rad(60.0f), 640.0f/480.0f, 0.1f, 1000.0f);

    Frustum camFrustum;
    camFrustum.SetAspect(640.0f/480.0f);
    camFrustum.SetFOV(Deg2Rad(60.0f));

    try
    {
        std::shared_ptr<IRenderer> pRndr = createRenderer();
        std::shared_ptr<AssimpMeshResourceLoader> pMeshLoader = std::make_shared<AssimpMeshResourceLoader>();
        std::shared_ptr<TextureResourceLoader> pTexLoader = std::make_shared<TextureResourceLoader>();
        ResourceZipFile* pZip = new ResourceZipFile("./resources.zip");
        std::shared_ptr<ResCache> pResCache = std::make_shared<ResCache>(50, pZip); // 50 MB of cache
        g_ResCache = pResCache.get();
        if (!g_ResCache->Init()) {
            throw std::runtime_error("g_ResCache init failed");
        }
        pResCache->RegisterLoader(pMeshLoader);
        pResCache->RegisterLoader(pTexLoader);
        std::shared_ptr<CameraNode> pCam = std::make_shared<CameraNode>(&Mat4x4::g_Identity, camFrustum);
        std::shared_ptr<OGLSkyNode> pSky = std::make_shared<OGLSkyNode>("skybox\\Sky2", pCam);
        std::shared_ptr<AssimpMeshNode> pMesh = std::make_shared<AssimpMeshNode>(
            ActorId(1), "My Mesh", "teapot.obj", RenderPass_Static, g_White, &modelMat);
        std::shared_ptr<Scene> pScene = std::make_shared<Scene>(pRndr);
        std::shared_ptr<LightNode> pLight = createLight();

        pRndr->VSetBackgroundColor(255,0,127,127);
        pRndr->VSetProjectionTransform(&projMat);
        pRndr->VSetViewTransform(&viewMat);
        pRndr->VSetWorldTransform(&modelMat);

        pMesh->VSetTransform(&modelMat);

        pCam->SetTarget(pMesh);
        pScene->SetCamera(pCam);
        pScene->AddChild(ActorId(42), pSky);
        pScene->AddChild(ActorId(1), pMesh);
        pScene->AddChild(ActorId(2), pLight);
        
        pCam->SetViewTransform(pScene.get());
        pMesh->VOnRestore(pScene.get());

        bool quit = false;
        float teapotRot = 0.0f;
        const float dt = 1.0f/60.0f;
        while (!quit)
        {
            quit = updateInput();
            Mat4x4 teapotXform =
                Translate(0.0f,-2.0f,-9.0f) *
                Rotate(teapotRot,Vec3(0.0f,1.0f,0.0f)) *
                Rotate(teapotRot*0.3f,Vec3(0.0f,0.0f,1.0f)) *
                Scale(0.05f,0.05f,0.05f);
            pMesh->VSetTransform(&teapotXform);
            teapotRot += M_PI * dt;

            pRndr->VPreRender();
            pScene->OnRender();
            pRndr->VPostRender();
        }
    }
    catch (std::exception& e)
    {
        printf("%s\n", e.what());
    }

    return 0;
}

