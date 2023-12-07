#ifndef GCC4_APP_MSG_H_INCLUDED
#define GCC4_APP_MSG_H_INCLUDED

#include <SDL2/SDL.h>

typedef SDL_Event AppMsg;

char AppMsgKeyToChar(const AppMsg& msg);

#endif // GCC4_APP_MSG_H_INCLUDED

