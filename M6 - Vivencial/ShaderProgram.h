#pragma once

#include <glad/glad.h>
#include "glm/glm.hpp"
#include <iostream>
#include <string>
#include <unordered_map>

class ShaderProgram {
private:

    GLuint rendererId;
    std::string vsFilepath;
    std::string fsFilepath;
    std::unordered_map<std::string, GLint> uniformLocations;

public:

    ShaderProgram(const std::string& vsFilepath, const std::string& fsFilepath);
    ~ShaderProgram();

    void bind() const;
    void unbind() const;
    void setUniform4f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
    void setUniform3f(const std::string& name, GLfloat v0, GLfloat v1, GLfloat v2);
    void setUniform1f(const std::string& name, GLfloat value);
    void setUniform1i(const std::string& name, GLint value);
    void setUniformMat4f(const std::string& name, const glm::mat4& matrix);
    GLuint getId() const;

private:

    std::string fileToString(const std::string& filepath) const;
    GLuint createShaderProgram(const std::string& vertexShader, const std::string& fragmentShader);
    GLuint compileShader(GLenum type, const std::string& source);
    GLint getUniformLocation(const std::string& name);

};

