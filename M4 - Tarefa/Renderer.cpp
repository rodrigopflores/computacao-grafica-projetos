#include "Renderer.h"

#include <iostream>

Renderer::Renderer(int width, int height) {

	if (!glfwInit()) {
		std::cout << "Falha ao inicilizar GLFW" << std::endl;
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	this->window = glfwCreateWindow(width, height, "Computação Gráfica - Unisinos", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(EXIT_FAILURE);
	}
	glEnable(GL_DEPTH_TEST);
}

GLFWwindow* Renderer::getWindow() const {
	return window;
}

void Renderer::loopSetup() {
	glfwPollEvents();

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	glClearColor(bgColor.r, bgColor.g, bgColor.b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setBgColor(glm::vec3 color) {
	this->bgColor = color;
}

bool Renderer::windowNotClosed() {
	return !glfwWindowShouldClose(this->window);
}

void Renderer::swap() {
	glfwSwapBuffers(this->window);
}

void Renderer::setKeyCallback(GLFWkeyfun callback) {
	glfwSetKeyCallback(this->window, callback);
}

void Renderer::setCursorCallback(GLFWcursorposfun callback) {
	glfwSetCursorPosCallback(this->window, callback);
}