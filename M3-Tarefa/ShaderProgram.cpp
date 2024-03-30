#include "ShaderProgram.h"
#include <fstream>
#include <sstream>

using namespace std;

ShaderProgram::ShaderProgram(const string &vsFilepath, const string &fsFilepath)
    : vsFilepath(vsFilepath), fsFilepath(fsFilepath) {
    string vertexShader = fileToString(vsFilepath);
    string fragmentShader = fileToString(fsFilepath);
    rendererId = createShaderProgram(vertexShader, fragmentShader);
}

ShaderProgram::~ShaderProgram() {
    glDeleteProgram(rendererId);
}

void ShaderProgram::bind() const {
    glUseProgram(rendererId);
}

void ShaderProgram::unbind() const {
    glUseProgram(0);
}

string ShaderProgram::fileToString(const std::string &filepath) const {
    ifstream stream(filepath);
    stringstream buffer;
    buffer << stream.rdbuf();

    return buffer.str();
}

GLuint ShaderProgram::createShaderProgram(const string &vertexShader, const string &fragmentShader) {
    GLuint program = glCreateProgram();
    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);
    // TODO Errorhandling
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

GLuint ShaderProgram::compileShader(GLenum type, const string &source) {
    GLuint id = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    GLint result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {
        GLint length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char *message = new char[length];
        glGetShaderInfoLog(id, length, &length, message);
        cout << "Failed compiling " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader:" << endl
             << message << endl;
        glDeleteShader(id);
        delete[] message;
        return 0;
    }

    return id;
}

GLint ShaderProgram::getUniformLocation(const string& name) {
    if (uniformLocations.find(name) != uniformLocations.end()) {
        return uniformLocations[name];
    }

    GLint location = glGetUniformLocation(rendererId, name.c_str());
    if (location == -1) {
        cout << "Warning: Uniform '" << name << "' doesn't exist" << endl;
    }
    uniformLocations[name] = location;
    return location;
}

void ShaderProgram::setUniform4f(const string &name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform4f(getUniformLocation(name), v0, v1, v2, v3);
}

void ShaderProgram::setUniform1i(const string &name, GLint value) {
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setUniformMat4f(const string &name, const glm::mat4 &matrix) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}
