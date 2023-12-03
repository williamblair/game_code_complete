#include "TextStyle.h"

TextStyle& TextStyle::operator=(const TextStyle& sp)
{
    m_font = sp.m_font;
    m_foreground = sp.m_foreground;
    m_background = sp.m_background;
    m_format = sp.m_format;
    m_pShadow = sp.m_pShadow;
    return *this;
}

