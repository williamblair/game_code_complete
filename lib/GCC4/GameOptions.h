#ifndef GCC4_GAME_OPTIONS_H_INCLUDED
#define GCC4_GAME_OPTIONS_H_INCLUDED

#include <string>
#include "Point.h"
#include "xml.h"

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
    
    tinyxml2::XMLDocument* m_pDoc;

    GameOptions();
    ~GameOptions() { if (m_pDoc) { delete m_pDoc; } }

    void Init(const char* xmlFilePath, int argc, char* argv[]);
};

#endif // GCC4_GAME_OPTIONS_H_INCLUDED

