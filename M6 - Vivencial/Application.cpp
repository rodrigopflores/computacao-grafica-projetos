#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <yaml-cpp/yaml.h>
#include <iostream>
#include <vector>
#include <string>

#include "Renderer.h"
#include "ShaderProgram.h"
#include "Object.h"
#include "Camera.h"
#include "Bezier.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
int getTrajectoryPoint(int timelapse, float millisPerPoint);

const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera;
std::vector<Object*> objects;
Object* selected = nullptr;

int main() {

	Renderer renderer(WIDTH, HEIGHT);
	renderer.setKeyCallback(keyCallback);
	renderer.setCursorCallback(cursorCallback);

	ShaderProgram program("resources/shaders/vs.glsl", "resources/shaders/fs.glsl");
	
	YAML::Node config = YAML::LoadFile("resources/config.yaml");

	for (const auto& objDescription : config["objects"]) {
		std::string objFile = objDescription["obj-file"].as<std::string>();
		std::vector<float> pos = objDescription["pos"].as<std::vector<float>>();
		std::vector<float> scale = objDescription["scale"].as<std::vector<float>>();
		float angle = objDescription["angle"].as<float>();

		Object* objPtr = new Object(objFile, program);
		objects.push_back(objPtr);
		Mesh& mesh = objPtr->getMesh();
		mesh.setPosition(glm::vec3(pos[0], pos[1], pos[2]));
		mesh.setAngle(angle);
		mesh.setScale(glm::vec3(scale[0], scale[1], scale[2]));
	}

	selected = objects[0];

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	program.setUniformMat4f("view", view);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	program.setUniformMat4f("projection", projection);

	program.setUniform3f("lightPos", -20.0, 20.0, -5.0);
	program.setUniform3f("lightColor", 1.0, 1.0, 1.0);

	while (renderer.windowNotClosed())
	{
		renderer.loopSetup();

		camera.updateShader(program);

		for (Object* object : objects) {
			if (object == selected) {
				auto& light = object->getLight();
				float& Ka = light["Ka"];
				Ka += 0.5f;
				object->draw();
				Ka -= 0.5f;
			} else {
				object->draw();
			}
		}
		renderer.swap();
	}

	glfwTerminate();
	return 0;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_LEFT_SHIFT) {
		selected->getMesh().getScale() += 0.05;
	}
	if (key == GLFW_KEY_LEFT_ALT) {
		selected->getMesh().getScale() -= 0.05;
	}
	if (key == GLFW_KEY_X) {
		selected->getMesh().setAxis(glm::vec3(1.0, 0.0, 0.0));
		selected->getMesh().getAngle() += 2.0;
	}
	if (key == GLFW_KEY_Y) {
		selected->getMesh().setAxis(glm::vec3(0.0, 1.0, 0.0));
		selected->getMesh().getAngle() += 2.0;
	}
	if (key == GLFW_KEY_Z) {
		selected->getMesh().setAxis(glm::vec3(0.0, 0.0, 1.0));
		selected->getMesh().getAngle() += 2.0;
	}
	float translationSpeed = 0.1;
	if (key == GLFW_KEY_J) {
		selected->getMesh().getPosition() += glm::vec3(-translationSpeed, 0.0, 0.0);
	}
	if (key == GLFW_KEY_L) {
		selected->getMesh().getPosition() += glm::vec3(translationSpeed, 0.0, 0.0);
	}
	if (key == GLFW_KEY_I) {
		selected->getMesh().getPosition() += glm::vec3(0.0, translationSpeed, 0.0);
	}
	if (key == GLFW_KEY_K) {
		selected->getMesh().getPosition() += glm::vec3(0.0, -translationSpeed, 0.0);
	}
	if (key == GLFW_KEY_BACKSLASH) {
		selected->getMesh().getPosition() += glm::vec3(0.0, 0.0, translationSpeed);
	}
	if (key == GLFW_KEY_RIGHT_BRACKET) {
		selected->getMesh().getPosition() += glm::vec3(0.0, 0.0, -translationSpeed);
	}
	if (key >= GLFW_KEY_0 && key <= GLFW_KEY_9) {
		int num = key - GLFW_KEY_0;
		if (num < objects.size()) {
			selected = objects[num];
		}
	}

	camera.translate(key, action);
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	camera.rotate(xpos, ypos);
}

int getTrajectoryPoint(int timelapse, float millisPerPoint) {
	return (int)(glfwGetTime() * 1000) % timelapse / millisPerPoint;
}