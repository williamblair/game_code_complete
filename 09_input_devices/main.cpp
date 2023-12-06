#include <chrono>
#include <SDL2/SDL.h>
#include <GCC4/Joystick.h>

// https://wiki.libsdl.org/SDL2/SDL_Event

#ifdef _WIN32
#undef main
#endif

int main()
{
    CheckForJoystick();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Sdl video init failed\n");
        return 1;
    }
    SDL_Window* pWindow = SDL_CreateWindow(
        "Test Window",
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        640, 480,
        0
    );

    const int numFrames = int(10000.0f/(1000.0f/60.0f));
    for (int i=0; i<numFrames; ++i)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                i = numFrames;
                break;
            case SDL_KEYDOWN:
            case SDL_KEYUP:
                printf("Keydown/up: %d, %d, %d\n",
                    (int)e.key.keysym.sym,
                    (int)e.key.state,
                    (int)e.key.repeat);
                break;
            case SDL_JOYAXISMOTION:
                printf("Joyaxis motion: %d, %d, %d\n",
                    (int)e.jaxis.which,
                    (int)e.jaxis.axis,
                    (int)e.jaxis.value);
                break;
            case SDL_JOYBALLMOTION:
                printf("Joyball motion: %d, %d, %d, %d\n",
                    (int)e.jball.which,
                    (int)e.jball.ball,
                    (int)e.jball.xrel,
                    (int)e.jball.yrel);
                break;
            case SDL_JOYHATMOTION:
                printf("Joyhat motion:\n",
                    (int)e.jhat.which,
                    (int)e.jhat.hat,
                    (int)e.jhat.value);
                break;
            case SDL_JOYBUTTONDOWN:
            case SDL_JOYBUTTONUP:
                printf("Joybutton down/up: %d, %d, %d\n",
                    (int)e.jbutton.which,
                    (int)e.jbutton.button,
                    (int)e.jbutton.state);
                break;
            case SDL_MOUSEMOTION:
                printf("Mousemotion: %d, %d, %d, %d\n",
                    (int)e.motion.x,
                    (int)e.motion.y,
                    (int)e.motion.xrel,
                    (int)e.motion.yrel);
                break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                printf("Mousebutton down/up: %d, %d, %d, %d, %d\n",
                    (int)e.button.button,
                    (int)e.button.state,
                    (int)e.button.clicks,
                    (int)e.button.x,
                    (int)e.button.y);
                break;
            default:
                break;
            }
        }

        SDL_Delay(int(1000.0f/60.0f));
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}

