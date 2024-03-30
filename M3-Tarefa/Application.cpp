#include <iostream>
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

using namespace std;

static void debugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
    cout << "[OpenGL] Source: 0x" << hex << source << " | Type: 0x" << type << " | Id: 0x" << id << " | Severity: 0x" << severity << endl;
    cout << "Message: " << message << endl;
    exit(-1);
}

int main() {

    if (!glfwInit()) {
        return -1;
    }

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 800, height = 600;

    GLFWwindow *window = glfwCreateWindow(width, height, "Computação Gráfica", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        return -1;
    }
    glfwSwapInterval(1);

    GLfloat positions[] = {
            // pos          // tex
            -0.5,-0.5,      0,0,
             0.5,-0.5,      1,0,
             0.5, 0.5,      1,1,
            -0.5, 0.5,      0,1
    };

    GLuint indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    ShaderProgram program("resources/shaders/sprite.vs", "resources/shaders/sprite.fs");
    glm::mat4 proj = glm::ortho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);
    program.setUniformMat4f("projection", proj);
    program.setUniformMat4f("model", glm::mat4());
    program.setUniform1i("tex_buffer", 0);
    Object bla("resources/3d_models/Suzanne/SuzanneTriTextured.obj", program);
    Texture texture("resources/3d_models/Suzanne/Suzanne.png");
    texture.bind();


    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback((GLDEBUGPROC)debugMessageCallback, nullptr);

    Renderer renderer;

    while (!glfwWindowShouldClose(window)) {
        renderer.clear();

        renderer.draw(bla);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}