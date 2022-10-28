#ifndef GCC4_OGL_SHADER_H_INCLUDED
#define GCC4_OGL_SHADER_H_INCLUDED

#include <string>
#include <GL/glew.h>

#include <GCCMath.h>

class OGLShader
{
public:
    OGLShader();
    ~OGLShader();

    bool LoadFromFile(const char* vertexFile, const char* shaderFile);

    void Use() { glUseProgram(mProgId); }

    bool SetMat4(const char* name, const Mat4x4& mat);
    bool SetVec3(const char* name, const Vec3& vec);
    bool SetVec2(const char* name, const Vec2& vec);
    bool SetInt(const char* name, int val);

private:
    GLuint mProgId;
    std::string getShaderStr(const std::string filename);
};

#endif
