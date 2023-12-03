#ifndef GCC4_ISCREENELEMENT_H_INCLUDED
#define GCC4_ISCREENELEMENT_H_INCLUDED

#include <list>
#include <memory>

class IScreenElement
{
public:

    virtual ~IScreenElement() {}

    //virtual HRESULT VOnRestore() = 0;
    //virtual HRESULT VOnLostDevice() = 0;
    virtual bool VOnRender(float fTime, float fElapsedTime) = 0;
    virtual void VOnUpdate(int deltaMilliseconds) = 0;
    
    virtual int VGetZOrder() const = 0;
    virtual void VSetZOrder(int const zOrder) = 0;
    virtual bool VIsVisible() const = 0;
    virtual void VSetVisible(bool visible) = 0;
    
    //virtual LRESULT CALLBACK VOnMsgProc(AppMsg msg) = 0;
    
    virtual bool const operator < (IScreenElement const& other) {
        return VGetZOrder() < other.VGetZOrder();
    }
};

typedef std::list<std::shared_ptr<IScreenElement>> ScreenElementList;

#endif // GCC4_ISCREENELEMENT_H_INCLUDED
