#include "VertexArray.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray() {
    glGenVertexArrays(1, &rendererId);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &rendererId);
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
    bind();
    vb.bind();
    const auto& elements = layout.getElements();
    GLuint offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);
        glEnableVertexAttribArray(i);
        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
    vb.unbind();
    unbind();
}

void VertexArray::bind() const {
    glBindVertexArray(rendererId);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}
