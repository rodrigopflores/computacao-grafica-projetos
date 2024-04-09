#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Object.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

const GLuint WIDTH = 800, HEIGHT = 600;
glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
bool rotateX = false, rotateY = false, rotateZ = false;
bool firstMouse = true;
float lastX, lastY;
float sensitivity = 0.05;
float pitch = 0.0, yaw = -90.0;

int main()
{
	Renderer renderer(WIDTH, HEIGHT);
	renderer.setBgColor(glm::vec3(0.8f, 0.8f, 0.8f));
	renderer.setKeyCallback(key_callback);
	renderer.setCursorCallback(mouse_callback);

	ShaderProgram program("resources/shaders/vs.glsl", "resources/shaders/fs.glsl");
	Texture planetTexture("../3D_Models/Planetas/2k_mercury.jpg");
	Object planet("../3D_Models/Planetas/planeta.obj", planetTexture, program);
	planet.getMesh().setScale(glm::vec3(0.5, 0.5, 0.5));
	planet.getMesh().setPosition(glm::vec3(-0.8, 0.0, 0.0));
	
	Texture suzanneTexture("../3D_Models/Suzanne/Suzanne.png");
	Object suzanne("../3D_Models/Suzanne/SuzanneTriTextured.obj", suzanneTexture, program);
	suzanne.getMesh().setScale(glm::vec3(0.5, 0.5, 0.5));
	suzanne.getMesh().setPosition(glm::vec3(0.8, 0.0, 0.0));

	Texture signTexture("../3D_Models/Novos/TexturasOffice.png");
	Object sign("../3D_Models/Novos/cienciaDaComputacao.obj", signTexture, program);
	sign.getMesh().setScale(glm::vec3(0.5, 0.5, 0.5));
	sign.getMesh().setPosition(glm::vec3(0.0, 1.0, -2.0));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	program.setUniformMat4f("view", view);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	program.setUniformMat4f("projection", projection);
	program.setUniform1f("q", 1.0);
	program.setUniform3f("lightPos", -2.0, 10.0, 2.0);
	program.setUniform3f("lightColor", 1.0, 1.0, 1.0);


	while (renderer.windowNotClosed())
	{
		renderer.loopSetup();

		program.setUniformMat4f("view", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
		program.setUniform3f("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);

		planet.draw();
		suzanne.draw();
		sign.draw();

		renderer.swap();
	}

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_X && action == GLFW_PRESS)
	{
		rotateX = true;
		rotateY = false;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}

	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = false;
		rotateZ = true;
	}

	float cameraSpeed = 0.05;

	if (key == GLFW_KEY_W)
	{
		cameraPos += cameraFront * cameraSpeed;
	}
	if (key == GLFW_KEY_S)
	{
		cameraPos -= cameraFront * cameraSpeed;
	}
	if (key == GLFW_KEY_A)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
	if (key == GLFW_KEY_D)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float offsetx = xpos - lastX;
	float offsety = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	offsetx *= sensitivity;
	offsety *= sensitivity;

	pitch += offsety;
	yaw += offsetx;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}
