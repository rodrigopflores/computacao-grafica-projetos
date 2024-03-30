#pragma once
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include <string>

class Object {
private:

	VertexArray va;
	IndexBuffer ib;
	ShaderProgram program;

public:

	Object(const VertexBuffer& vb, const VertexBufferLayout& layout, const IndexBuffer& ib, const ShaderProgram& program);
	Object(const std::string& filepath, const ShaderProgram& programv);

	void bind() const;
	GLuint getCount() const;
};