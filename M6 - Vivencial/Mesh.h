#pragma once
#include "glm/glm.hpp"

class Mesh {
private:

	glm::vec3 position;
	glm::vec3 scale;
	float angle;
	glm::vec3 axis;

public:

	Mesh();
	void setPosition(glm::vec3 position);
	void setAxis(glm::vec3 axis);
	void setAngle(float angle);
	void setScale(glm::vec3 scale);
	glm::mat4 getUpdatedMesh() const;
	glm::vec3& getScale();
	float& getAngle();
	glm::vec3& getPosition();
};

