#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

void Camera::rotate(double posX, double posY) {
	
	lastX = lastX < 0 ? posX : lastX;
	lastY = lastY < 0 ? posY : lastY;

	float sensitivity = 0.1;
	float offsetx = posX - lastX;
	float offsety = lastY - posY;

	lastX = posX;
	lastY = posY;

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

void Camera::translate(int key, int action) {

	float cameraSpeed = 0.1;

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

void Camera::updateShader(ShaderProgram& program) {
	program.setUniformMat4f("view", glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp));
	program.setUniform3f("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
}