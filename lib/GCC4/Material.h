#ifndef GCC4_MATERIAL_H_INCLUDED
#define GCC4_MATERIAL_H_INCLUDED

#include <cstring>

#include "Color.h"

class Material
{
public:

    Material()
    {
        m_Diffuse.Set(1.0f,1.0f,1.0f,1.0f);
        m_Ambient.Set(0.1f,0.1f,0.1f,1.0f);
        m_Specular.Set(1.0f,1.0f,1.0f,1.0f);
        m_Emissive.Set(0.0f,0.0f,0.0f,1.0f);
        m_Power = 0.0f;
    }
    
    void SetAmbient(const Color& color) { m_Ambient = color; }
    Color GetAmbient() { return m_Ambient; }
    
    void SetDiffuse(const Color& color) { m_Diffuse = color; }
    Color GetDiffuse() { return m_Diffuse; }
    
    void SetSpecular(const Color& color, const float power) {
        m_Specular = color; m_Power = power;
    }
    void GetSpecular(Color& _color, float& _power) {
        _color = m_Specular; _power = m_Power;
    }
    
    void SetEmissive(const Color& color) { m_Emissive = color; }
    const Color GetEmissive() { return m_Emissive; }
    
    void SetAlpha(const float alpha) { m_Diffuse.a = alpha; }
    bool HasAlpha() const { return (m_Diffuse.a < 1.0f); }
    float GetAlpha() const { return m_Diffuse.a; }
    
    
    Material& operator=(const Material& m) {
        m_Diffuse = m.m_Diffuse;
        m_Ambient = m.m_Ambient;
        m_Specular = m.m_Specular;
        m_Emissive = m.m_Emissive;
        m_Power = m.m_Power;
        return *this;
    }
    
private:
    Color m_Ambient;
    Color m_Diffuse;
    Color m_Specular;
    Color m_Emissive;
    float m_Power;
};

#endif // GCC4_MATERIAL_H_INCLUDED
