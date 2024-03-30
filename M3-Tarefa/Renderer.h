#ifndef OPENGLSANDBOX_RENDERER_H
#define OPENGLSANDBOX_RENDERER_H

#include <GL/glew.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "Object.h"

#define ASSERT(x) if(!(x)) __debugbreak();
#define glCall(x) glClearErrors(); \
                  x;               \
                  ASSERT(glLogCall(#x, __FILE__, __LINE__));

void glClearErrors();
bool glLogCall(const char *function, const char *file, int line);

class Renderer {
public:
    void clear() const;
    void draw(const VertexArray& va, const IndexBuffer& ib, const ShaderProgram& program) const;
    void draw(const Object& object) const;
};


#endif //OPENGLSANDBOX_RENDERER_H
