#include <cstdio>
#include <memory>
#include <stdexcept>

#include <SDL2/SDL.h>

#include <GCCMath.h>
#include <IRenderer.h>
#include <Scene.h>
#include <OGLRenderer.h>
#include <OGLVertexBuffer.h>
#include <OGLSkyNode.h>

//std::shared_ptr<OGLRenderer>
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

int main(int argc, char **argv)
{
    Mat4x4 modelMat = Translate(0.0f,0.0f,0.0f);
    Mat4x4 viewMat = Mat4x4::g_Identity;
    Mat4x4 projMat = Perspective(Deg2Rad(60.0f), 640.0f/480.0f, 0.1f, 1000.0f);

    Frustum camFrustum;
    camFrustum.SetAspect(640.0f/480.0f);
    camFrustum.SetFOV(Deg2Rad(60.0f));

    try
    {
        std::shared_ptr<IRenderer> pRndr = createRenderer();
        std::shared_ptr<CameraNode> pCam = std::make_shared<CameraNode>(&modelMat, camFrustum);
        std::shared_ptr<OGLSkyNode> pSky = std::make_shared<OGLSkyNode>("../skybox/skybox", pCam);
        std::shared_ptr<Scene> pScene = std::make_shared<Scene>(pRndr);

        pRndr->VSetBackgroundColor(255,0,127,127);
        pRndr->VSetProjectionTransform(&projMat);
        pRndr->VSetViewTransform(&viewMat);
        pRndr->VSetWorldTransform(&modelMat);

        pScene->SetCamera(pCam);
        pScene->AddChild(ActorId(42), pSky);

        bool quit = false;
        while (!quit)
        {
            quit = updateInput();

            pRndr->VPreRender();
            //pSky->VPreRender(pScene.get());
            //pSky->VRender(pScene.get());
            //pSky->VPostRender(pScene.get());
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
