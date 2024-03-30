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


public:

	Object(const std::string& filepath, const Texture& texture);
	GLsizei getSize() const;
	void bind();
	void unbind();
	void draw();

};

