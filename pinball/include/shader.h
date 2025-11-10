#pragma once

#include <string>
#include <GL/glew.h>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    Shader(const std::string& vertexSource, const std::string& fragmentSource, bool isSourceCode);
    ~Shader();
    
    void use();
    
    // Set uniforms
    void setFloat(const std::string& name, float value);
    void setInt(const std::string& name, int value);
    void setVec2(const std::string& name, float x, float y);
    void setVec3(const std::string& name, float x, float y, float z);
    void setVec4(const std::string& name, float x, float y, float z, float w);
    void setMat4(const std::string& name, const float* mat4);
    
    GLuint getID() const { return ID; }
    
private:
    GLuint ID;
    
    std::string loadShaderSource(const std::string& filePath);
    void compileShader(const std::string& vertexCode, const std::string& fragmentCode);
    void checkCompileErrors(GLuint shader, std::string type);
};