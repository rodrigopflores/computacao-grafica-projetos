#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Object.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
	Renderer renderer(WIDTH, HEIGHT);
	renderer.setBgColor(glm::vec3(0.8f, 0.8f, 0.8f));
	GLFWwindow* window = renderer.getWindow();
	glfwSetKeyCallback(window, key_callback);

	ShaderProgram program("resources/shaders/vs.glsl", "resources/shaders/fs.glsl");
	Texture texture("../3D_Models/Suzanne/Suzanne.png");
	Object object("../3D_Models/Suzanne/SuzanneTriTextured.obj", texture);

	program.bind();
	program.setUniform1i("tex_buffer", 0);
	glm::mat4 projection = glm::ortho(-3.0, 3.0, -3.0, 3.0, -1.0, 1.0);
	program.setUniformMat4f("projection", projection);

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		renderer.loopSetup();

		glm::mat4 model = glm::rotate(glm::mat4(1), (float)glfwGetTime(), glm::vec3(0, 1, 0));
		program.setUniformMat4f("model", model);

		object.draw();

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
