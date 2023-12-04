#ifndef GCC4_SCREEN_ELEMENT_SCENE_H_INCLUDED
#define GCC4_SCREEN_ELEMENT_SCENE_H_INCLUDED

#include "IScreenElement.h"
#include "Scene.h"

class ScreenElementScene : public IScreenElement, public Scene
{
public:

    ScreenElementScene(std::shared_ptr<IRenderer> pRenderer) :
        Scene(pRenderer)
    {}
    virtual ~ScreenElementScene()
    {
        printf("~ScreenElementScene\n");
    }
    
    // IScreenElement implementation
    virtual void VOnUpdate(int deltaMs) { OnUpdate(deltaMs); }
    //TODO
    //virtual HRESULT VOnRestore() {
    //    OnRestore();
    //    return S_OK;
    //}
    virtual bool VOnRender(float fTime, float fElapsedTime) {
        OnRender();
        return true;
    }
    //TODO
    //virtual HRESULT VOnLostDevice() {
    //    OnLostDevice();
    //    return S_OK;
    //}
    virtual int VGetZOrder() const { return 0; }
    virtual void VSetZOrder(int const) {}
    
    // Don't handle any messages
    //TODO
    //virtual HRESULT CALLBACK VOnMsgProc(AppMsg msg) { return 0; }
    
    virtual bool VIsVisible() const { return true; }
    virtual void VSetVisible(bool) {}
    virtual bool VAddChild(ActorId id, std::shared_ptr<ISceneNode> kid) {
        Scene::AddChild(id, kid);
    }
};

#endif // GCC4_SCREEN_ELEMENT_SCENE_H_INCLUDED
