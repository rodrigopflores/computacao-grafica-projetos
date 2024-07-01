#include "Mesh.h"
#include "glm/gtc/matrix_transform.hpp"

Mesh::Mesh() {
	this->position = glm::vec3(0.0, 0.0, 0.0);
	this->scale = glm::vec3(1.0, 1.0, 1.0);
	this->angle = 0.0;
	this->axis = glm::vec3(0.0, 1.0, 0.0);
}

glm::mat4 Mesh::getUpdatedMesh() const {
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, this->position);
	model = glm::rotate(model, glm::radians(this->angle), axis);

	return glm::scale(model, this->scale);
}

void Mesh::setAngle(float angle) {
	this->angle = angle;
}

void Mesh::setAxis(glm::vec3 axis) {
	this->axis = axis;
}

void Mesh::setPosition(glm::vec3 position) {
	this->position = position;
}

void Mesh::setScale(glm::vec3 scale) {
	this->scale = scale;
}

glm::vec3& Mesh::getScale() {
	return this->scale;
}

float& Mesh::getAngle() {
	return this->angle;
}

glm::vec3& Mesh::getPosition() {
	return this->position;
}