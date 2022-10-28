#include <fstream>

#include "OGLShader.h"

OGLShader::OGLShader() :
    mProgId(0)
{}

OGLShader::~OGLShader()
{
    if (mProgId) {
        glDeleteProgram(mProgId);
    }
}

bool OGLShader::LoadFromFile(const char* vertexFile, const char* fragmentFile)
{
    // get the string data of both files
    std::string vFileStr = getShaderStr(vertexFile);
    std::string fFileStr = getShaderStr(fragmentFile);

    // verify the shader files were opened correctly
    if (vFileStr.empty() || fFileStr.empty()) {
        return false;
    }

    // initialze each shader
    GLuint vertexShaderID   = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    const char* shaderSrc = vFileStr.c_str();

    // send and compile the vertex shader
    glShaderSource(vertexShaderID, 1, &shaderSrc, NULL);
    glCompileShader(vertexShaderID);

    // check for success
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShaderID, 512, NULL, infoLog);
        printf("Shader::Shader: Vertex Shader Compilation failed: %s\n", infoLog);
        return false;
    }

    shaderSrc = fFileStr.c_str();
    
    // send and compile the fragment shader
    glShaderSource(fragmentShaderID, 1, &shaderSrc, NULL);
    glCompileShader(fragmentShaderID);

    // check for success
    glGetShaderiv(fragmentShaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShaderID, 512, NULL, infoLog);
        printf("Shader::Shader: Fragment Shader Compilation failed: %s\n", infoLog);
        return false;
    }

    // link the shaders to the main program
    mProgId = glCreateProgram();
    glAttachShader(mProgId, vertexShaderID);
    glAttachShader(mProgId, fragmentShaderID);
    glLinkProgram(mProgId);

    // check for success
    glGetShaderiv(mProgId, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(mProgId, 512, NULL, infoLog);
        printf("Shader::Shader: Program Linkage Failed: %s\n", infoLog);
        return false;
    }

    // delete our component shaders now
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    
    return true;
}

bool OGLShader::SetMat4(const char* name, const Mat4x4& mat)
{
    GLint pos = glGetUniformLocation(this->mProgId, name);
    if (pos < 0) {
        //printf("Failed to find mat4 uniform: %s\n", name);
        return false;
    }
        
    glUniformMatrix4fv(pos, 1, GL_FALSE, (float*)&mat);
    return true;
}
bool OGLShader::SetVec3(const char* name, const Vec3& vec)
{
    GLint pos = glGetUniformLocation(this->mProgId, name);
    if (pos < 0) {
        //printf("Failed to find mat4 uniform: %s\n", name);
        return false;
    }
        
    glUniform3fv(pos, 1, (float*)&vec);
    return true;
}
bool OGLShader::SetVec2(const char* name, const Vec2& vec)
{
    GLint pos = glGetUniformLocation(this->mProgId, name);
    if (pos < 0) {
        //printf("Failed to find mat4 uniform: %s\n", name);
        return false;
    }
        
    glUniform2fv(pos, 1, (float*)&vec);
    return true;
}
bool OGLShader::SetInt(const char* name, int val)
{
    GLint pos = glGetUniformLocation(this->mProgId, name);
    if (pos < 0) {
        //printf("Failed to find mat4 uniform: %s\n", name);
        return false;
    }
        
    glUniform1i(pos, val);
    return true;
}

std::string OGLShader::getShaderStr(const std::string filename)
{
    std::ifstream f(filename, std::ios::in);
    if (!f.is_open()) {
        printf("Shader::getShaderStr: failed to open %s\n", filename.c_str());
        return "";
    }

    std::string str((std::istreambuf_iterator<char>(f)),
                 std::istreambuf_iterator<char>());

    return str;
}

