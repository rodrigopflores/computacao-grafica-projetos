#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

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
	earth.getMesh().setPosition(glm::vec3(0.0, -1.0, -3.0));
	earth.getMesh().setScale(glm::vec3(0.5,0.5,0.5));

	Texture unisinosTexture("../3D_Models/Novos/TexturasOffice.png");
	Object unisinos("../3D_Models/Novos/unisinos.obj", unisinosTexture, program);
	unisinos.getMesh().setPosition(glm::vec3(0.0, 1.0, -3.0));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	program.setUniformMat4f("view", view);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	program.setUniformMat4f("projection", projection);

	program.setUniform1f("q", 1.0);
	program.setUniform3f("lightPos", -20.0, 20.0, -5.0);

	program.setUniform3f("lightColor", 1.0, 1.0, 1.0);

	std::ifstream file("resources/pos.csv"); 
	std::string line;
	std::vector<float> earthPos;

	if (!file.is_open()) {
		std::cerr << "Não foi possível abrir arquivo." << std::endl;
		exit(-1);
	}
	while (std::getline(file, line)) {
		std::stringstream ss(line);
		std::string value;

		while (std::getline(ss, value, ',')) {
			earthPos.push_back(std::stof(value));
		}
	}
	file.close();
	

	int numberOfPoints = earthPos.size()/3;
	int stepMillis = 20;
	while (renderer.windowNotClosed())
	{
		renderer.loopSetup();

		camera.updateShader(program);
		int timeMillis = glfwGetTime() * 1000.0;
		int xCoor = (timeMillis/stepMillis % numberOfPoints) * 3;
		glm::vec3 pos(earthPos[xCoor], earthPos[xCoor+1], earthPos[xCoor+2]);
		earth.getMesh().setPosition(pos);
		earth.draw();
		unisinos.draw();

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
