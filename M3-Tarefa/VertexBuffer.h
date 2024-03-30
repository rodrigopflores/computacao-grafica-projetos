#ifndef OPENGLSANDBOX_VERTEXBUFFER_H
#define OPENGLSANDBOX_VERTEXBUFFER_H

#include "GL/glew.h"

class VertexBuffer {
private:
    GLuint rendererId;

public:
    VertexBuffer(const void* data, GLuint size);
    ~VertexBuffer();

    void bind() const;
    void unbind() const;
};


#endif //OPENGLSANDBOX_VERTEXBUFFER_H
