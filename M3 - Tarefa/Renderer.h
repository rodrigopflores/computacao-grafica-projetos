#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Renderer {
private:

	GLFWwindow* window;
	glm::vec3 bgColor;

public:

	Renderer(int width, int height);
	GLFWwindow* getWindow() const;
	void loopSetup();
	void setBgColor(glm::vec3 color);

};

