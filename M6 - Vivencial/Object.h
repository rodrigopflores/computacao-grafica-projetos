#pragma once

#include <unordered_map>
#include <string>
#include <glad/glad.h>
#include "glm/glm.hpp"
#include "VertexArray.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Mesh.h"

class Object {
private:
	
	VertexArray va;
	GLsizei size;
	ShaderProgram program;
	Texture texture;
	std::unordered_map<std::string, float> light;
	Mesh mesh;

	void loadObjectFromFile(const std::string& filepath);
	void loadMaterial(std::string filepath);
	void update();

public:

	Object(const std::string& filepath, const Texture& texture, const ShaderProgram& program);
	Mesh& getMesh();
	std::unordered_map<std::string, float>& getLight();
	void bind();
	void unbind();
	void draw();

};

