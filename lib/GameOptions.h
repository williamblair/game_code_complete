#ifndef GCC4_GAME_OPTIONS_H_INCLUDED
#define GCC4_GAME_OPTIONS_H_INCLUDED

#include <string>
#include "xml.h"

struct Point
{
    int x,y;

    Point() :
        x(0),y(0)
    {}
    Point(int _x, int _y) :
        x(_x),y(_y)
    {}
};

struct GameOptions
{
    std::string m_Level;

    std::string m_Renderer;
    bool m_bRunFullSpeed;
    Point m_ScreenSize;

    float m_SoundEffectsVolume;
    float m_MusicVolume;

    int m_ExpectedPlayers;
    int m_ListenPort;
    std::string m_GameHost;
    int m_NumAis;
    int m_MaxAis;
    int m_MaxPlayers;
    
    bool m_bUseDevelopmentDirectories;
    
    XMLDocument* m_pDoc;

    GameOptions();
    ~GameOptions() { if (m_pDoc) { delete m_pDoc; } }

    void Init(const char* xmlFilePath, int argc, char* argv[]);
};

#endif // GCC4_GAME_OPTIONS_H_INCLUDED

