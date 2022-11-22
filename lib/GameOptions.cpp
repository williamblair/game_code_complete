#include <stdexcept>
#include "GameOptions.h"

GameOptions::GameOptions()
{
    m_Level = "";
    m_Renderer = "OpenGL";
    m_bRunFullSpeed = false;
    m_SoundEffectsVolume = 1.0f;
    m_MusicVolume = 1.0f;
    m_ExpectedPlayers = 1;
    m_ListenPort = -1;
    std::string m_GameHost = "MrMike-m1710";
    m_NumAis = 1;
    m_MaxAis = 4;
    m_MaxPlayers = 4;
    m_ScreenSize = Point(1024,768); 
    m_bUseDevelopmentDirectories = false;
    m_pDoc = nullptr;
}

void GameOptions::Init(const char* xmlFilePath, int argc, char* argv[])
{
    // read the XML file
    // if needed, override the XML file with options passed in
    // on the command line
    m_pDoc = new XMLDocument();
    if (m_pDoc && (m_pDoc->LoadFile(xmlFilePath)==XML_SUCCESS))
    {
        XMLElement* pRoot = m_pDoc->RootElement();
        if (!pRoot) {
            return;
        }

        // Loop through each child element and load the component
        XMLElement* pNode = nullptr;
        pNode = pRoot->FirstChildElement("Graphics");
        if (pNode) {
            std::string attribute;
            attribute = pNode->Attribute("renderer");
            if (attribute != "OpenGL") {
                throw std::runtime_error("Invalid renderer setting in graphics options.");
            }
            m_Renderer = attribute;

            if (pNode->Attribute("width")) {
                m_ScreenSize.x = atoi(pNode->Attribute("width"));
                if (m_ScreenSize.x < 800) { m_ScreenSize.x = 800; }
            }

            if (pNode->Attribute("height")) {
                m_ScreenSize.y = atoi(pNode->Attribute("height"));
                if (m_ScreenSize.y < 600) { m_ScreenSize.y = 600; }
            }

            if (pNode->Attribute("runfullspeed")) {
                attribute = pNode->Attribute("runfullspeed");
                m_bRunFullSpeed = (attribute == "yes");
            }
        }

        pNode = pRoot->FirstChildElement("Sound");
        if (pNode) {
            m_MusicVolume = atoi(pNode->Attribute("musicVolume")) / 100.0f;
            m_SoundEffectsVolume = atoi(pNode->Attribute("sfxVolume")) / 100.0f;
        }

        pNode = pRoot->FirstChildElement("Multiplayer");
        if (pNode) {
            m_ExpectedPlayers = atoi(pNode->Attribute("expectedPlayers"));
            m_NumAis = atoi(pNode->Attribute("numAIs"));
            m_MaxAis = atoi(pNode->Attribute("maxAIs"));
            m_MaxPlayers = atoi(pNode->Attribute("maxPlayers"));
            
            m_ListenPort = atoi(pNode->Attribute("listenPort"));
            m_GameHost = pNode->Attribute("gameHost");
        }

        pNode = pRoot->FirstChildElement("ResCache");
        if (pNode) {
            std::string attribute(pNode->Attribute("useDevelopmentDirectories"));
            m_bUseDevelopmentDirectories = (attribute == "yes");
        }
    }
}

