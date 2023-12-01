#ifndef GCC4_IGAMEVIEW_H_INCLUDED
#define GCC4_IGAMEVIEW_H_INCLUDED

#include "ComponentTypes.h"

typedef unsigned int GameViewId;
#define gc_InvalidGameViewId 0xFFFFFFFF

enum GameViewType
{
    GameView_Human,
    GameView_Remote,
    GameView_AI,
    GameView_Recorder,
    GameView_Other
};

class IGameView
{
public:

    virtual ~IGameView() {}

    //virtual HRESULT VOnRestore() = 0;
    virtual void VOnRender(float fTime, float fElapsedTime) = 0;
    //virtual HRESULT VOnLostDevice() = 0;
    virtual GameViewType VGetType() = 0;
    virtual GameViewId VGetId() const = 0;
    virtual void VOnAttach(GameViewId vid, ActorId aid) = 0;

    //virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
    virtual void VOnUpdate(unsigned long deltaMs) = 0;
};

#endif // GCC4_IGAMEVIEW_H_INCLUDED

