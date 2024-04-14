#pragma once

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
	Mesh mesh;

	void loadObjectFromFile(const std::string& filepath);
	void loadMaterial(std::string filepath);
	void updateModel();

public:

	Object(const std::string& filepath, const Texture& texture, const ShaderProgram& program);
	Mesh& getMesh();
	void bind();
	void unbind();
	void draw();

};

