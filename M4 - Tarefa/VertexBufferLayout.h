#pragma once

#include <glad/glad.h>
#include <vector>

#define ASSERT(x) if(!(x)) __debugbreak();

struct VertexBufferElement {
    GLuint type;
    GLuint count;
    GLuint normalized;

    static GLuint getSizeOfType(GLuint type) {
        switch (type) {
        case GL_FLOAT: return sizeof(GLfloat);
        case GL_UNSIGNED_INT: return sizeof(GLuint);
        case GL_UNSIGNED_BYTE: return sizeof(GLubyte);
        default:
            ASSERT(false);
            return 0;
        }
    }
};

class VertexBufferLayout {
private:
    std::vector<VertexBufferElement> elements;
    GLuint stride;
public:
    explicit VertexBufferLayout() : stride(0) {}

    template<typename T>
    void push(GLuint count) {
       // static_assert(false);
    }

    template<>
    void push<GLfloat>(GLuint count) {
        elements.push_back({ GL_FLOAT, count, GL_FALSE });
        stride += sizeof(GLfloat) * count;
    }

    template<>
    void push<GLuint>(GLuint count) {
        elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
        stride += sizeof(GLuint) * count;
    }

    template<>
    void push<GLubyte>(GLuint count) {
        elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
        stride += sizeof(GLubyte) * count;
    }

    const std::vector<VertexBufferElement>& getElements() const {
        return elements;
    }

    GLuint getStride() const {
        return stride;
    }
};
