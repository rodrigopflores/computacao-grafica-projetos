#pragma once

#include <glad/glad.h>

class VertexBuffer {
private:

    GLuint rendererId;

public:

    VertexBuffer(const void* data, GLuint size);
    ~VertexBuffer();
    void bind() const;
    void unbind() const;

};
