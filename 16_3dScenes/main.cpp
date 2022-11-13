#include <cstdio>
#include <memory>
#include <stdexcept>

#include <SDL2/SDL.h>

#include <GCCMath.h>
#include <IRenderer.h>
#include <Scene.h>
#include <ResCache.h>
#include <ResourceZipFile.h>
#include <AssimpMeshResourceLoader.h>
#include <TextureResourceLoader.h>
#include <AssimpMeshNode.h>
#include <OGLRenderer.h>
#include <OGLVertexBuffer.h>
#include <OGLSkyNode.h>

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

ResCache* g_ResCache = nullptr;

#ifdef _WIN32
#undef main
#endif

int main(int argc, char **argv)
{
    Mat4x4 modelMat = Translate(0.0f,0.0f,-5.0f)*Scale(0.01f,0.01f,0.01f);
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

        pRndr->VSetBackgroundColor(255,0,127,127);
        pRndr->VSetProjectionTransform(&projMat);
        pRndr->VSetViewTransform(&viewMat);
        pRndr->VSetWorldTransform(&modelMat);

        pMesh->SetPosition(Vec3(0.0f,0.0f,-20.0f));
        pMesh->VSetTransform(&modelMat);

        pCam->SetTarget(pMesh);
        pScene->SetCamera(pCam);
        pScene->AddChild(ActorId(42), pSky);
        pScene->AddChild(ActorId(1), pMesh);
        
        pCam->SetViewTransform(pScene.get());
        pMesh->VOnRestore(pScene.get());

        bool quit = false;
        while (!quit)
        {
            quit = updateInput();

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
