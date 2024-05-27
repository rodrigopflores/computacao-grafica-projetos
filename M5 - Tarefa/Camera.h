#pragma once

#include "glm/glm.hpp"
#include "ShaderProgram.h"

class Camera {
private:

	float lastX = -1.0, lastY = -1.0;
	float pitch = 0.0, yaw = -90.0;
	glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 5.0);
	glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
	glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);

public:

	void rotate(double posX, double posY);
	void translate(int key, int action);
	void updateShader(ShaderProgram& program);

};

