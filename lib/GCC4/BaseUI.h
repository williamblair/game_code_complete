#ifndef GCC4_BASE_UI_H_INCLUDED
#define GCC4_BASE_UI_H_INCLUDED

#include "IScreenElement.h"

enum ZOrder
{
    // Lower numbers get drawn first
    ZORDER_BACKGROUND,
    ZORDER_LAYER1,
    ZORDER_LAYER2,
    ZORDER_LAYER3,
    ZORDER_TOOLTIP
};

enum MessageBoxQuestions
{
    QUESTION_WHERES_THE_CD,
    QUESTION_QUIT_GAME
};

class BaseUI : public IScreenElement
{
public:

    BaseUI() :
        m_PosX(0),
        m_PosY(0),
        m_Width(100),
        m_Height(100),
        m_Result(nullptr),
        m_bIsVisible(true)
    {}

    virtual void VOnUpdate(int) {}
    //TODO
    //virtual HRESULT VOnLostDevice() { return S_OK; }
    virtual bool VIsVisible() const { return m_bIsVisible; }
    virtual void VSetVisible(bool bVisible) { m_bIsVisible = bVisible; }

protected:
    int m_PosX, m_PosY;
    int m_Width, m_Height;
    int* m_Result;
    bool m_bIsVisible;
};

#endif // GCC4_BASE_UI_H_INCLUDED
