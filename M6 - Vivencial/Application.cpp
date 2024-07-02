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
void loadConfigFromYaml(ShaderProgram& program);

const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera;
std::vector<Object*> objects;
Object* selected = nullptr;
struct Animation {
	Bezier trajectory;
	Object* object;
};
std::vector<Animation> animations;

int main() {

	Renderer renderer(WIDTH, HEIGHT);
	renderer.setKeyCallback(keyCallback);
	renderer.setCursorCallback(cursorCallback);

	ShaderProgram program("resources/shaders/vs.glsl", "resources/shaders/fs.glsl");
	
	loadConfigFromYaml(program);

	selected = objects[0];

	int trajTimelapse = 4000;
	float millisPerPoint = (float)trajTimelapse / 1000;

	while (renderer.windowNotClosed())
	{
		renderer.loopSetup();

		camera.updateShader(program);

		for (auto& animation : animations) {
			glm::vec3 pos = animation.trajectory.getPointOnCurve(getTrajectoryPoint(trajTimelapse, millisPerPoint));
			animation.object->getMesh().setPosition(pos);
		}

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

	for (Object* object : objects) {
		delete object;
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
	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
		int index = find(objects.begin(), objects.end(), selected) - objects.begin();
		selected = objects[++index % objects.size()];
	}
	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
		int index = find(objects.begin(), objects.end(), selected) - objects.begin();
		int i = --index >= 0 ? index : index + objects.size();
		selected = objects[i];
	}

	camera.translate(key, action);
}

void cursorCallback(GLFWwindow* window, double xpos, double ypos) {
	camera.rotate(xpos, ypos);
}

void loadConfigFromYaml(ShaderProgram& program) {

	YAML::Node config = YAML::LoadFile("resources/config.yaml");

	auto cameraConfig = config["camera"];
	camera.setCameraPos(cameraConfig["pos"].as<glm::vec3>());
	camera.setCameraFront(cameraConfig["front"].as<glm::vec3>());
	camera.setCameraUp(cameraConfig["up"].as<glm::vec3>());

	auto projAngle = config["projection"]["angle"].as<float>();
	auto frustum = config["projection"]["frustum"].as<std::vector<float>>();
	glm::mat4 projection = glm::perspective(glm::radians(projAngle), (float)WIDTH / (float)HEIGHT, frustum[0], frustum[1]);
	program.setUniformMat4f("projection", projection);

	auto lightPos = config["light"]["pos"].as<glm::vec3>();
	program.setUniform3f("lightPos", lightPos.x, lightPos.y, lightPos.z);

	auto lightColor = config["light"]["color"].as<glm::vec3>();
	program.setUniform3f("lightColor", lightColor.x, lightColor.y, lightColor.z);

	for (const auto& objDescription : config["objects"]) {
		std::string objFile = objDescription["obj-file"].as<std::string>();
		glm::vec3 pos = objDescription["pos"].as<glm::vec3>();
		glm::vec3 scale = objDescription["scale"].as<glm::vec3>();
		float angle = objDescription["angle"].as<float>();
		Object* objPtr = new Object(objFile, program);
		objects.push_back(objPtr);
		Mesh& mesh = objPtr->getMesh();
		mesh.setPosition(glm::vec3(pos.x, pos.y, pos.z));
		mesh.setAngle(angle);
		mesh.setScale(glm::vec3(scale.x, scale.y, scale.z));

		auto& animationNode = objDescription["animation"];
		if (animationNode) {
			std::vector<glm::vec3> controlPoints = animationNode.as<std::vector<glm::vec3>>();
			Animation animation;
			animation.trajectory.setControlPoints(controlPoints);
			animation.trajectory.generateCurve(1000);
			animation.object = objPtr;
			animations.push_back(animation);
		}
	}
}

int getTrajectoryPoint(int timelapse, float millisPerPoint) {
	return (int)(glfwGetTime() * 1000) % timelapse / millisPerPoint;
}

namespace YAML {
	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3) {
				return false;
			}

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}