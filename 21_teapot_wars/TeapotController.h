#ifndef TEAPOT_CONTROLLER_H_INCLUDED
#define TEAPOT_CONTROLLER_H_INCLUDED

#include <GCC4/IPointerHandler.h>
#include <GCC4/IKeyboardHandler.h>
#include <GCC4/SceneNode.h>

class TeapotController : public IPointerHandler, public IKeyboardHandler
{
public:

    TeapotController(std::shared_ptr<SceneNode> object);


    virtual bool VOnPointerMove(const Point& mousePos, const int radius) { return true; }
    virtual bool VOnPointerButtonDown(const Point& mousePos, const int radius, const std::string& buttonName);
    virtual bool VOnPointerButtonUp(const Point& mousePos, const int radius, const std::string& buttonName) {
        return buttonName == "PointerLeft";
    }

    virtual bool VOnKeyDown(const char c);
    virtual bool VOnKeyUp(const char c);

    void OnUpdate(unsigned long elapsedMs);

protected:
    char m_bKey[256];
    std::shared_ptr<SceneNode> m_object;
};

#endif // TEAPOT_CONTROLLER_H_INCLUDED

