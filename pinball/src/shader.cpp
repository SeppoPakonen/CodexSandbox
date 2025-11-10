#include "shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);
    
    compileShader(vertexCode, fragmentCode);
}

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource, bool isSourceCode) {
    if (isSourceCode) {
        compileShader(vertexSource, fragmentSource);
    } else {
        std::string vertexCode = loadShaderSource(vertexSource);
        std::string fragmentCode = loadShaderSource(fragmentSource);
        compileShader(vertexCode, fragmentCode);
    }
}

Shader::~Shader() {
    glDeleteProgram(ID);
}

std::string Shader::loadShaderSource(const std::string& filePath) {
    std::ifstream file;
    std::stringstream stream;
    
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    
    try {
        file.open(filePath);
        stream << file.rdbuf();
        file.close();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << filePath << std::endl;
        throw std::runtime_error("Failed to load shader file: " + filePath);
    }
    
    return stream.str();
}

void Shader::compileShader(const std::string& vertexCode, const std::string& fragmentCode) {
    const char* vCode = vertexCode.c_str();
    const char* fCode = fragmentCode.c_str();
    
    // Compile vertex shader
    GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    
    // Compile fragment shader
    GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    
    // Create shader program
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    
    // Delete shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::checkCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" 
                << infoLog << std::endl;
            throw std::runtime_error("Shader compilation error: " + type);
        }
    } else {
        glGetProgramiv(ID, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(ID, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" 
                << infoLog << std::endl;
            throw std::runtime_error("Program linking error: " + type);
        }
    }
}

void Shader::use() {
    glUseProgram(ID);
}

void Shader::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setInt(const std::string& name, int value) {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setVec2(const std::string& name, float x, float y) {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setMat4(const std::string& name, const float* mat4) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, mat4);
}