#include "Sprite.h"

Sprite::Sprite()
{
    //TODO
}

Sprite::~Sprite()
{
    //TODO
}

void Sprite::VOnUpdate(int deltaMs)
{
    //TODO
}

bool Sprite::VOnRender(float fTime, float fElapsedTime)
{
    //TODO
    return true;
}



BitmapSprite::BitmapSprite(const std::string& fileName, bool inCache, const int frames)
{
    //TODO
}




TextSprite::TextSprite(
    const std::string& text,
    TextStyle style,
    bool isCentered,
    const Point& position,
    Point* m_pSize)
{
    //TODO
}

bool TextSprite::VOnRender(float fTime, float fElapsedTime)
{
    //TODO
    return true;
}

void TextSprite::SetText(const std::string text)
{
    //TODO
}

