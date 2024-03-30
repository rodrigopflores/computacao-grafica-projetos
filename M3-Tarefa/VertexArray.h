#ifndef OPENGLSANDBOX_VERTEXARRAY_H
#define OPENGLSANDBOX_VERTEXARRAY_H

#include "GL/glew.h"
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


#endif //OPENGLSANDBOX_VERTEXARRAY_H
