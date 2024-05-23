#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const GLuint* data, GLuint count) : count(count) {
    glGenBuffers(1, &rendererId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &rendererId);
}

void IndexBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererId);
}

void IndexBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint IndexBuffer::getCount() const {
    return count;
}
