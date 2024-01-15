#ifndef GCC4_SPRITE_H_INCLUDED
#define GCC4_SPRITE_H_INCLUDED

#include <string>

#include "IScreenElement.h"
#include "Point.h"
#include "Rect.h"
#include "TextStyle.h"
#include "OGLShader.h"
#include "OGLVertexBuffer.h"

class Sprite : public IScreenElement
{
public:

    Sprite();
    virtual ~Sprite();

    // IScreenElement implementation
    virtual void VOnUpdate(int deltaMs);
    virtual bool VOnRender(float fTime, float fElapsedTime);
    virtual int VGetZOrder() const { return m_ZOrder; }
    virtual void VSetzOrder(int const zOrder) { m_ZOrder = zOrder; }
    virtual bool VIsVisible() const { return m_IsVisible; }
    virtual void VSetVisible(bool visible) { m_IsVisible = visible; }
    virtual bool VOnMsgProc(AppMsg msg) { return 0; }
    //TODO
    //virtual HRESULT VOnRestore();


    virtual Point const VGetPos() const { return m_Position; }
    virtual void VSetPos(const Point& loc) { m_Position = loc; }
    virtual void VSetHotspot(const Point& loc) { m_Hotspot = loc; }
    virtual void VSetSize(const Point& size) { m_Width = size.x; m_Height = size.y; }
    virtual Rect const VGetRect() const { return Rect(0,0,m_Width-1,m_Height-1); }
    virtual int const VGetWidth() const { return m_Width; }
    virtual int const VGetHeight() const { return m_Height; }

    //TODO
    //void* GetSurface() { return m_d3dSprite; }

    void SetFrame(const int desiredFrame) { m_CurrentFrame = desiredFrame % m_NumFrames; }
    int GetFrame() const { return m_CurrentFrame; }
    int GetFrameCount() const { return m_NumFrames; }
    void SetAnim(bool isPaused = false, bool looping = true, int msPerFrame = 67) {
        m_IsPaused = isPaused;
        m_LoopingAnim = looping;
        m_MSPerFrame = msPerFrame;
    }

protected:
    //TODO
    //ID3DXSprite* m_d3dSprite;
    //IDirect3DTexture9* m_pTexture;
    unsigned int m_Texture; // GLuint texture

    // position and pixel data
    Point m_Position, m_Hotspot; // subtract HS from pos to get origin
    int m_ZOrder;
    int m_Width, m_Height;
    int m_TextureWidth, m_TextureHeight;
    int m_CurrentFrame, m_NumFrames;
    bool m_HasColorKey;
    bool m_IsVisible;

    // Members that control animation
    bool m_IsPaused;
    bool m_LoopingAnim;
    int m_ElapsedTime;
    int m_MSPerFrame; // 1000 / desired frames per second

    static unsigned int s_SpriteCount;
    static OGLShader* s_pSpriteShader;
    static OGLVertexBuffer* s_SpriteVertBuf;
    static bool InitSpriteShader();
    static bool InitSpriteVertBuf();
};

class BitmapSprite : public Sprite
{
public:

    BitmapSprite(const std::string& fileName, bool inCache, const int frames);

    //TODO
    //virtual HRESULT VOnRestore();

private:
    std::string m_FileName;
    bool m_bInCache;
};

class TextSprite : public Sprite
{
public:

    TextSprite(
        const std::string& text,
        TextStyle style,
        bool isCentered,
        const Point& position,
        Point* pSize
    );

    //TODO
    //virtual HRESULT VOnRestore();

    bool VOnRender(float fTime, float fElapsedTime);

    void SetText(const std::string text);

private:
    std::string m_Text;
    TextStyle m_Style;
    bool m_bIsCentered;
    Point* m_pSize;
};

#endif // GCC4_SPRITE_H_INCLUDED
