#include "Object.h"
#include <fstream>
#include <sstream>
#include <vector>
#include "VertexBufferLayout.h"

Object::Object(const VertexBuffer& vb, const VertexBufferLayout& layout, const IndexBuffer& ib, const ShaderProgram& program) : ib(ib), program(program) {
	va.addBuffer(vb, layout);
	bind();
};

Object::Object(const std::string& filepath, const ShaderProgram& program) : program(program) {

	std::vector <glm::vec3> vertices;
	std::vector <GLuint> indices;
	std::vector <glm::vec2> texCoords;
	std::vector <glm::vec3> normals;
	std::vector <GLfloat> vbuffer;

	std::ifstream stream(filepath);

	for (std::string line; std::getline(stream, line); ) {
		

		std::istringstream ssline(line);
		std::string firstWord;
		ssline >> firstWord;

		if (firstWord == "v") {
			glm::vec3 v;
			ssline >> v.x >> v.y >> v.z;

			vertices.push_back(v);
		}
		else if (firstWord == "vt") {
			glm::vec2 vt;
			ssline >> vt.s >> vt.t;

			texCoords.push_back(vt);
		}
		else if (firstWord == "vn") {
			glm::vec3 vn;
			ssline >> vn.x >> vn.y >> vn.z;

			normals.push_back(vn);
		}
		else if (firstWord == "f") {
			std::string tokens[3];

			ssline >> tokens[0] >> tokens[1] >> tokens[2];

			for (int i = 0; i < 3; i++)
			{

				int pos = tokens[i].find("/");
				std::string token = tokens[i].substr(0, pos);
				int index = atoi(token.c_str()) - 1;
				indices.push_back(index);

				vbuffer.push_back(vertices[index].x);
				vbuffer.push_back(vertices[index].y);
				vbuffer.push_back(vertices[index].z);
				vbuffer.push_back(0.0);
				vbuffer.push_back(0.0);
				vbuffer.push_back(0.0);

				tokens[i] = tokens[i].substr(pos + 1);
				pos = tokens[i].find("/");
				token = tokens[i].substr(0, pos);
				index = atoi(token.c_str()) - 1;
				vbuffer.push_back(texCoords[index].s);
				vbuffer.push_back(texCoords[index].t);

				tokens[i] = tokens[i].substr(pos + 1);
				index = atoi(tokens[i].c_str()) - 1;
				vbuffer.push_back(normals[index].x);
				vbuffer.push_back(normals[index].y);
				vbuffer.push_back(normals[index].z);
			}
		}
		else {
			std::cout << "Falha ao decodificar o arquivo " << filepath << std::endl;
		}
	}
	stream.close();
	VertexBuffer vb(vbuffer.data(), vbuffer.size() * sizeof(float));
	VertexBufferLayout layout;
	layout.push<GLfloat>(3);
	layout.push<GLfloat>(3);
	layout.push<GLfloat>(2);
	layout.push<GLfloat>(3);
	std::cout << (vbuffer.size() / 11) << std::endl;
	va.addBuffer(vb, layout);
	ib.setOrderedIndices(vbuffer.size() / 11);
}

void Object::bind() const {
	va.bind();
	ib.bind();
	program.bind();
}

GLuint Object::getCount() const {
	return ib.getCount();
}
