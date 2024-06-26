#include "Object.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "glm/gtc/matrix_transform.hpp"
using namespace std;

Object::Object(const std::string& filepath, const ShaderProgram& program) : program(program) {
	string folder = filepath.substr(0, filepath.find_last_of("/")) + "/" ;
	string materialFile;
	string textureFile;
	loadObjectFromFile(filepath, materialFile);
	loadMaterial(folder + materialFile, textureFile);
	texture.load(folder + textureFile);
}

void Object::loadObjectFromFile(const std::string& filepath, std::string& materialFile) {

	cout << "Carregando objeto de arquivo .obj: " << filepath << endl;
	vector <glm::vec3> vertices;
	vector <GLuint> indices;
	vector <glm::vec2> texCoords;
	vector <glm::vec3> normals;
	vector <GLfloat> vbuffer;

	ifstream inputFile;
	inputFile.open(filepath.c_str());

	if (!inputFile.is_open()) {
		cout << "Problema ao encontrar o arquivo " << filepath << endl;
		exit(EXIT_FAILURE);
	}

	char line[100];
	string sline;
	while (!inputFile.eof()) {
		inputFile.getline(line, 100);
		sline = line;

		string word;

		istringstream ssline(line);
		ssline >> word;
		if (word == "o") {
			string name;
			ssline >> name;
			cout << "Nome do objeto: " << name << endl;
		}
		if (word == "usemtl") {
			string name;
			ssline >> name;
			cout << "Nome do material: " << name << endl;
		}
		if (word == "mtllib") {
			ssline >> materialFile;
		}
		if (word == "v") {
			glm::vec3 v;
			ssline >> v.x >> v.y >> v.z;

			vertices.push_back(v);
		}
		if (word == "vt") {
			glm::vec2 vt;
			ssline >> vt.s >> vt.t;

			texCoords.push_back(vt);
		}
		if (word == "vn") {
			glm::vec3 vn;
			ssline >> vn.x >> vn.y >> vn.z;
			normals.push_back(vn);
		}
		if (word == "f") {
			string tokens[3];

			ssline >> tokens[0] >> tokens[1] >> tokens[2];

			for (int i = 0; i < 3; i++) {
				//Recuperando os indices de v
				int pos = tokens[i].find("/");
				string token = tokens[i].substr(0, pos);
				int index = atoi(token.c_str()) - 1;
				indices.push_back(index);

				vbuffer.push_back(vertices[index].x);
				vbuffer.push_back(vertices[index].y);
				vbuffer.push_back(vertices[index].z);
				vbuffer.push_back(0.75);
				vbuffer.push_back(0.6);
				vbuffer.push_back(0.2);

				//Recuperando os indices de vts
				tokens[i] = tokens[i].substr(pos + 1);
				pos = tokens[i].find("/");
				token = tokens[i].substr(0, pos);
				index = atoi(token.c_str()) - 1;
				vbuffer.push_back(texCoords[index].s);
				vbuffer.push_back(texCoords[index].t);
				//Recuperando os indices de vns
				tokens[i] = tokens[i].substr(pos + 1);
				index = atoi(tokens[i].c_str()) - 1;
				vbuffer.push_back(normals[index].x);
				vbuffer.push_back(normals[index].y);
				vbuffer.push_back(normals[index].z);
			}
		}
	}

	inputFile.close();

	this->size = vbuffer.size() / 11;
	VertexBuffer vb(vbuffer.data(), vbuffer.size() * sizeof(GLfloat));
	VertexBufferLayout layout;

	layout.push<GLfloat>(3);
	layout.push<GLfloat>(3);
	layout.push<GLfloat>(2);
	layout.push<GLfloat>(3);
	this->va.addBuffer(vb, layout);
}

void Object::loadMaterial(std::string filepath, std::string& textureFile) {

	cout << "Carregando material do arquivo .mtl: " << filepath << endl;
	ifstream inputFile;
	inputFile.open(filepath.c_str());

	if (!inputFile.is_open()) {
		cout << "Problema ao encontrar o arquivo " << filepath << endl;
		exit(EXIT_FAILURE);
	}

	char line[100];
	string sline;

	while (!inputFile.eof()) {
		inputFile.getline(line, 100);
		sline = line;

		string word;

		istringstream ssline(line);
		ssline >> word;
		if (word == "newmtl") {
			string name;
			ssline >> name;
			cout << "Nome do material: " << name << endl;
		} else if (word == "map_Kd") {
			ssline >> textureFile;
		} else if (word == "Ka" || word == "Kd" || word == "Ks" || word == "Ns") {
			float value;
			if (!(ssline >> value)) {
				cerr << "Error: Failed to convert " << word << " value to float." << endl;
				continue;
			}
			std::cout << "Atribuindo a seguinte propriedade de luz a partir do arquivo mtl: " << word << " = " << value << std::endl;
			this->light[word] = value;
		}
	}
	inputFile.close();
}

Mesh& Object::getMesh() {
	return this->mesh;
}

void Object::update() {
	program.setUniform1f("Ka", this->light["Ka"]);
	program.setUniform1f("Kd", this->light["Kd"]);
	program.setUniform1f("Ks", this->light["Ks"]);
	program.setUniform1f("q", this->light["Ns"]);
	glm::mat4 updatedMesh = this->mesh.getUpdatedMesh();
	program.setUniformMat4f("model", updatedMesh);
	glm::mat4 normalMatrix = glm::transpose(glm::inverse(updatedMesh));
	program.setUniformMat4f("normalMatrix", normalMatrix);
}

void Object::bind() const {
	va.bind();
	texture.bind();
}

void Object::unbind() const {
	va.unbind();
	texture.unbind();
}

void Object::draw() {
	update();
	bind();
	glDrawArrays(GL_TRIANGLES, 0, this->size);
	unbind();
}

std::unordered_map<std::string, float>& Object::getLight() {
	return this->light;
}