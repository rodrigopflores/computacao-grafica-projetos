#pragma once

#include <glad/glad.h>
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray {
private:

    GLuint rendererId;

public:

    VertexArray();
    ~VertexArray();

    void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    void bind() const;
    void unbind() const;

};
