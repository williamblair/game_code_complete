#include <cstdio>
#include <stdexcept>

#include <SDL2/SDL.h>

#include <GCCMath.h>
#include <IRenderer.h>
#include <OGLRenderer.h>

void testPlane()
{
    Plane plane;
    plane.Init(Vec3(1,2,-2), Vec3(3,-2,1), Vec3(5,1,-4));
    //plane.Init(Vec3(5,1,-4), Vec3(3,-2,1), Vec3(1,2,2));
    printf("Plane abcd: %f,%f,%f,%f\n", plane.a, plane.b, plane.c, plane.d);
}

std::unique_ptr<IRenderer>
createRenderer()
{
    std::unique_ptr<OGLRenderer> rndr =
        std::make_unique<OGLRenderer>();
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
    printf("hello world\n");
    testPlane();

    try
    {
        std::unique_ptr<IRenderer> rndr = createRenderer();
        rndr->VSetBackgroundColor(255,0,127,127);

        bool quit = false;
        while (!quit)
        {
            quit = updateInput();

            rndr->VPreRender();
            rndr->VPostRender();
        }
    }
    catch (std::exception& e)
    {
        printf("%s\n", e.what());
    }

    return 0;
}
