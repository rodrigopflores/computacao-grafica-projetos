#pragma once

#include <string>
#include <glad/glad.h>
#include "VertexArray.h"
#include "Texture.h"

class Object {
private:
	
	VertexArray va;
	GLsizei size;
	std::string materialName;
	Texture texture;

	void loadMaterial(std::string filepath);

public:

	Object(const std::string& filepath, const Texture& texture);
	void bind();
	void unbind();
	void draw();

};

