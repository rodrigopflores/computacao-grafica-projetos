#pragma once
#include <glad/glad.h>

class IndexBuffer {

private:

    GLuint rendererId;
    GLuint count;

public:

    IndexBuffer(const GLuint* data, GLuint count);
    ~IndexBuffer();
    void bind() const;
    void unbind() const;
    GLuint getCount() const;

};
