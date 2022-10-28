#include <cstdio>
#include <memory>
#include <stdexcept>

#include <SDL2/SDL.h>

#include <GCCMath.h>
#include <IRenderer.h>
#include <OGLRenderer.h>
#include <OGLVertexBuffer.h>

std::unique_ptr<OGLRenderer>
createRenderer()
{
    std::unique_ptr<OGLRenderer> rndr =
        std::make_unique<OGLRenderer>();
    if (!rndr->Init(640,480)) {
        throw std::runtime_error("Failed to init renderer");
    }
    return rndr;
}

std::unique_ptr<OGLVertexBuffer>
createVertBuf()
{
    OGLVertexBuffer::VertexColored testVerts[3] = {
        // xyz                  nxnynz              rgb
        { -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 0.0f },
        { 0.0f, 0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 1.0f, 0.0f },
        { 0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 1.0f }
    };
    std::unique_ptr<OGLVertexBuffer> pVertBuf = std::make_unique<OGLVertexBuffer>();
    if (!pVertBuf->Init(
            OGLVertexBuffer::POS_COLOR,
            testVerts, 3,
            nullptr, 0,
            OGLVertexBuffer::USAGE_STATIC)) {
        throw std::runtime_error("Failed to init vert buf\n");
    }
    return pVertBuf;
}

std::unique_ptr<OGLShader>
createShader()
{
    std::unique_ptr<OGLShader> pShdr = std::make_unique<OGLShader>();
    if (!pShdr->LoadFromFile("shaders/coloredVertex.glsl", "shaders/coloredFragment.glsl")) {
        throw std::runtime_error("Failed to init colored shader");
    }
    return pShdr;
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
    Mat4x4 modelMat = Translate(0.0f,0.0f,-5.0f);
    Mat4x4 viewMat = Mat4x4::g_Identity;
    Mat4x4 projMat = Perspective(Deg2Rad(60.0f), 640.0f/480.0f, 0.1f, 1000.0f);

    try
    {
        std::unique_ptr<OGLRenderer> pRndr = createRenderer();
        std::unique_ptr<OGLVertexBuffer> pVertBuf = createVertBuf();
        std::unique_ptr<OGLShader> pShdr = createShader();

        pRndr->VSetBackgroundColor(255,0,127,127);
        pRndr->VSetProjectionTransform(&projMat);
        pRndr->VSetViewTransform(&viewMat);
        pRndr->VSetWorldTransform(&modelMat);

        bool quit = false;
        while (!quit)
        {
            quit = updateInput();

            pRndr->VPreRender();
            pRndr->SetShader(*pShdr);
            pRndr->DrawVertexBuffer(*pVertBuf);
            pRndr->VPostRender();
        }
    }
    catch (std::exception& e)
    {
        printf("%s\n", e.what());
    }

    return 0;
}
