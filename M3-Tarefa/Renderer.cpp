#include "Renderer.h"

#include <iostream>

using namespace std;

void glClearErrors() {
    while (glGetError() != GL_NO_ERROR);
}

bool glLogCall(const char *function, const char *file, int line) {
    while (GLenum error = glGetError()) {
        cout << "[GL Error] (" << error << ") " << function << " " << file << ":" << line << endl;
        return false;
    }
    return true;
}

void Renderer::draw(const VertexArray &va, const IndexBuffer &ib, const ShaderProgram& program) const {
    program.bind();
    va.bind();
    ib.bind();
    glDrawElements(GL_TRIANGLES, ib.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::draw(const Object& object) const {
    object.bind();
    glDrawElements(GL_TRIANGLES, object.getCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::clear() const {
    glClear(GL_COLOR_BUFFER_BIT);
}
