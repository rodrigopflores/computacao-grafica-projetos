#ifndef OPENGLSANDBOX_INDEXBUFFER_H
#define OPENGLSANDBOX_INDEXBUFFER_H
#include "GL/glew.h"

class IndexBuffer {
private:
    GLuint rendererId;
    GLuint count;
public:
    IndexBuffer();
    IndexBuffer(const GLuint* data, GLuint count);
    ~IndexBuffer();

    void setOrderedIndices(GLuint count);
    void bind() const;
    void unbind() const;
    GLuint getCount() const;
};


#endif //OPENGLSANDBOX_INDEXBUFFER_H
