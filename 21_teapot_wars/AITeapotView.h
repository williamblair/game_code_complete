#ifndef AI_TEAPOT_VIEW_H_INCLUDED
#define AI_TEAPOT_VIEW_H_INCLUDED

#include <GCC4/PathingGraph.h>
#include <GCC4/IGameView.h>
#include <GCC4/Logger.h>

class AITeapotView : public IGameView
{
    friend class AITeapotViewListener;

public:
    AITeapotView(std::shared_ptr<PathingGraph> pPathingGraph);
    virtual ~AITeapotView();

    virtual bool VOnRestore() { return true; }
    virtual void VOnRender(float fTime, float fElapsedTime) {}
    virtual bool VOnLostDevice() { return true; }
    virtual GameViewType VGetType() { return GameView_AI; }
    virtual GameViewId VGetId() const { return m_ViewId; }
    virtual void VOnAttach(GameViewId vid, ActorId actorId) {
        m_ViewId = vid;
        m_PlayerActorId = actorId;
    }
    virtual bool VOnMsgProc(AppMsg msg) { return false; }
    virtual void VOnUpdate(unsigned long deltaMs) {}

    std::shared_ptr<PathingGraph> GetPathingGraph() const { return m_pPathingGraph; }

protected:
    GameViewId m_ViewId;
    ActorId m_PlayerActorId;
private:
    std::shared_ptr<PathingGraph> m_pPathingGraph;
};

#endif // AI_TEAPOT_VIEW_H_INCLUDED

