#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Object.h"
#include "Camera.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);

const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera;

int main()
{
	Renderer renderer(WIDTH, HEIGHT);
	renderer.setKeyCallback(keyCallback);
	renderer.setCursorCallback(cursorCallback);

	ShaderProgram program("resources/shaders/vs.glsl", "resources/shaders/fs.glsl");
	Texture earthTexture("../3D_Models/Planetas/Terra.jpg");
	Object earth("../3D_Models/Planetas/planeta.obj", earthTexture, program);
	earth.getMesh().setPosition(glm::vec3(0.0, 0.0, -3.0));

	Texture mercuryTexture("../3D_Models/Planetas/2k_mercury.jpg");
	Object mercury("../3D_Models/Planetas/planeta.obj", mercuryTexture, program);
	mercury.getMesh().setScale(glm::vec3(0.38, 0.38, 0.38));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	program.setUniformMat4f("view", view);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	program.setUniformMat4f("projection", projection);
	program.setUniform3f("lightPos", -2.0, 5.0, 2.0);
	program.setUniform3f("lightColor", 1.0, 1.0, 1.0);


	while (renderer.windowNotClosed())
	{
		renderer.loopSetup();

		camera.updateShader(program);
		mercury.getMesh().setAngle(glfwGetTime()*40);
		earth.draw();
		mercury.draw();

		renderer.swap();
	}

	glfwTerminate();
	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	camera.translate(key, action);
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	camera.rotate(xpos, ypos);
}
