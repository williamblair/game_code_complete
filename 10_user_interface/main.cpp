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
#include <GCC4/Sprite.h>

#ifdef _WIN32
#undef main
#endif

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
    Mat4x4 modelMat = Translate(0.0f,-2.0f,-9.0f)*Scale(0.05f,0.05f,0.05f);
    Mat4x4 viewMat = Mat4x4::g_Identity;
    Mat4x4 projMat = Perspective(Deg2Rad(60.0f), 640.0f/480.0f, 0.1f, 1000.0f);

    std::shared_ptr<IRenderer> pRndr = createRenderer();
    std::shared_ptr<TextureResourceLoader> pTexLoader = std::make_shared<TextureResourceLoader>();
    ResourceZipFile* pZip = new ResourceZipFile("./resources.zip");
    std::shared_ptr<ResCache> pResCache = std::make_shared<ResCache>(50, pZip);
    g_pResCache = pResCache.get();
    if (!g_pResCache->Init()) {
        throw std::runtime_error("g_pResCache init failed");
    }
    g_pResCache->RegisterLoader(pTexLoader);

    pRndr->VSetBackgroundColor(255,0,127,127);
    pRndr->VSetProjectionTransform(&projMat);
    pRndr->VSetViewTransform(&viewMat);
    pRndr->VSetWorldTransform(&modelMat);

    std::shared_ptr<BitmapSprite> pSprite = std::make_shared<BitmapSprite>("skybox\\Sky2_back.jpg", false, 1);

    bool quit = false;
    while (!quit)
    {
        quit = updateInput();
        pRndr->VPreRender();
        pSprite->VOnRender(0.0f,0.0f);
        pRndr->VPostRender();
    }

    return 0;
}

