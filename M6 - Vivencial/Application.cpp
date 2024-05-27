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
#include "Bezier.h"

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursorCallback(GLFWwindow* window, double xpos, double ypos);
int getTrajectoryPoint(int timelapse, float millisPerPoint);

const GLuint WIDTH = 800, HEIGHT = 600;
Camera camera;

int main()
{

	float distance = -10.0;
	Renderer renderer(WIDTH, HEIGHT);
	renderer.setKeyCallback(keyCallback);
	renderer.setCursorCallback(cursorCallback);

	ShaderProgram program("resources/shaders/vs.glsl", "resources/shaders/fs.glsl");
	Texture earthTexture("../3D_Models/Planetas/Terra.jpg");
	Object earth("../3D_Models/Planetas/planeta.obj", earthTexture, program);
	earth.getMesh().setPosition(glm::vec3(0.0, 0.0, distance));
	//earth.getMesh().setScale(glm::vec3(0.5,0.5,0.5));
	
	Texture moonTexture("../3D_Models/Planetas/2k_mercury.jpg");
	Object moon("../3D_Models/Planetas/planeta.obj", moonTexture, program);
	moon.getMesh().setScale(glm::vec3(0.5, 0.5, 0.5));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	program.setUniformMat4f("view", view);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	program.setUniformMat4f("projection", projection);

	program.setUniform3f("lightPos", -20.0, 20.0, -5.0);
	program.setUniform3f("lightColor", 1.0, 1.0, 1.0);

	Bezier trajectory;
	float radius = 5.0;

	vector<glm::vec3> controlPoints = {
		glm::vec3(-radius, 0.0, distance),
		glm::vec3(-radius, radius, distance),
		glm::vec3(radius, radius, distance),
		glm::vec3(radius, 0.0, distance),
		glm::vec3(radius, -radius, distance),
		glm::vec3(-radius, -radius, distance),
		glm::vec3(-radius, 0.0, distance)
	};

	trajectory.setControlPoints(controlPoints);
	trajectory.generateCurve(1000);
	int planetaNbCurvePoints = trajectory.getNbCurvePoints();
	int trajTimelapse = 4000;
	float millisPerPoint = (float)trajTimelapse / planetaNbCurvePoints;

	while (renderer.windowNotClosed())
	{
		renderer.loopSetup();

		camera.updateShader(program);
		int trajectoryPoint = getTrajectoryPoint(trajTimelapse, millisPerPoint);
		moon.getMesh().setPosition(trajectory.getPointOnCurve(trajectoryPoint));
		moon.draw();
		earth.draw();

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

int getTrajectoryPoint(int timelapse, float millisPerPoint) {
	return (int)(glfwGetTime() * 1000) % timelapse / millisPerPoint;
}