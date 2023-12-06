#include <map>
#include "AppMsg.h"

char AppMsgKeyToChar(const AppMsg& msg)
{
    static std::map<SDL_Keycode,char> keyMap = {
        {SDLK_a,'A'},
        {SDLK_b,'B'},
        {SDLK_c,'C'},
        {SDLK_d,'D'},
        {SDLK_e,'E'},
        {SDLK_f,'F'},
        {SDLK_g,'G'},
        {SDLK_h,'H'},
        {SDLK_i,'I'},
        {SDLK_j,'J'},
        {SDLK_k,'K'},
        {SDLK_l,'L'},
        {SDLK_m,'M'},
        {SDLK_n,'N'},
        {SDLK_o,'O'},
        {SDLK_p,'P'},
        {SDLK_q,'Q'},
        {SDLK_r,'R'},
        {SDLK_s,'S'},
        {SDLK_t,'T'},
        {SDLK_u,'U'},
        {SDLK_v,'V'},
        {SDLK_w,'W'},
        {SDLK_x,'X'},
        {SDLK_y,'Y'},
        {SDLK_z,'Z'},
        {SDLK_0,'0'},
        {SDLK_1,'1'},
        {SDLK_2,'2'},
        {SDLK_3,'3'},
        {SDLK_4,'4'},
        {SDLK_5,'5'},
        {SDLK_6,'6'},
        {SDLK_7,'7'},
        {SDLK_8,'8'},
        {SDLK_9,'9'},
        {SDLK_SPACE,' '},
        {SDLK_BACKSPACE,(char)8}, // ascii
        {SDLK_RETURN,(char)10}, // new line
        {SDLK_ESCAPE,(char)27}, // escape
        {SDLK_TAB,(char)9} // horizontal tab
    };
    if (keyMap.find(msg.key.keysym.sym) != keyMap.end()) {
        return keyMap.at(msg.key.keysym.sym);
    }
    return (char)msg.key.keysym.sym;
}

