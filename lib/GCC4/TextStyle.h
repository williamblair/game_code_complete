#ifndef GCC4_TEXT_STYLE_H_INCLUDED
#define GCC4_TEXT_STYLE_H_INCLUDED

#include "Color.h"
#include "Point.h"

enum FontEnum
{
    FONT_DEFAULT,
    FONT_SYSTEM = FONT_DEFAULT,     //0
    FONT_ARIAL_BOLD_8,              //1
    FONT_ARIAL_BOLD_14,             //2
    FONT_ARIAL_BLACK_12,            //3
    FONT_ARIAL_BLACK_24,            //4
    FONT_WINGDING_10,               //5
    FONT_COURIERNEW_18,             //6
    FONT_PALATINO_SMALL,            //7
    FONT_PALATINO_MEDIUM,           //8
    FONT_PALATINO_LARGE,            //9
    FONT_LARGE_LED_DISPLAY,         //10
    FONT_TREBUCHET_10,              //11
    FONT_TREBUCHET_MED,             //12
    FONT_TAHOMA_MED,                //13
    FONT_TAHOMA_LARGE,              //14

    MAX_FONTS                       //15
};

struct TextShadow
{
    Color m_color;
    Point m_offset;
    
    TextShadow(Color const& color, Point const& offset = Point(1,1)) :
        m_color(color), m_offset(offset)
    {}
};

#ifndef DT_TOP
#define DT_TOP  0x01
#define DT_LEFT 0x02
#endif

struct TextStyle
{
    FontEnum m_font;
    Color m_foreground;
    Color m_background;
    int m_format;
    TextShadow* m_pShadow;
    
    TextStyle() :
        m_font(FONT_DEFAULT),
        m_foreground(),
        m_background(),
        m_format(0),
        m_pShadow(nullptr)
    {}
    
    TextStyle(
            FontEnum font,
            Color foreground,
            Color background = g_Transparent,
            int format = DT_TOP | DT_LEFT,
            TextShadow* pShadow = nullptr) :
        m_font(font),
        m_foreground(foreground),
        m_background(background),
        m_format(format),
        m_pShadow(pShadow)
    {}
    
    TextStyle& operator=(const TextStyle& sp);
};

#endif // GCC4_TEXT_STYLE_H_INCLUDED
