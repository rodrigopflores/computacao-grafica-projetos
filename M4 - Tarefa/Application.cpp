#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>

#include "Renderer.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Object.h"

const GLuint WIDTH = 800, HEIGHT = 600;

int main()
{
	Renderer renderer(WIDTH, HEIGHT);
	renderer.setBgColor(glm::vec3(0.6f, 0.6f, 0.6f));

	ShaderProgram program("resources/shaders/vs.glsl", "resources/shaders/fs.glsl");
	Texture planetTexture("../3D_Models/Planetas/2k_mercury.jpg");
	Object planet("../3D_Models/Planetas/planeta.obj", planetTexture, program);
	planet.getMesh().setScale(glm::vec3(0.5, 0.5, 0.5));
	planet.getMesh().setPosition(glm::vec3(-0.8, 0.0, 0.0));
	
	Texture suzanneTexture("../3D_Models/Suzanne/Suzanne.png");
	Object suzanne("../3D_Models/Suzanne/SuzanneTriTextured.obj", suzanneTexture, program);
	suzanne.getMesh().setScale(glm::vec3(0.5, 0.5, 0.5));
	suzanne.getMesh().setPosition(glm::vec3(0.8, 0.0, 0.0));

	Texture signTexture("../3D_Models/Novos/TexturasOffice.png");
	Object sign("../3D_Models/Novos/cienciaDaComputacao.obj", signTexture, program);
	sign.getMesh().setScale(glm::vec3(0.5, 0.5, 0.5));
	sign.getMesh().setPosition(glm::vec3(0.0, 1.0, -2.0));

	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	program.setUniformMat4f("view", view);
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
	program.setUniformMat4f("projection", projection);
	program.setUniform1f("q", 1.0);
	program.setUniform3f("lightPos", -2.0, 10.0, 2.0);
	program.setUniform3f("lightColor", 1.0, 1.0, 1.0);


	while (renderer.windowNotClosed())
	{
		renderer.loopSetup();

		planet.draw();
		suzanne.draw();
		sign.draw();

		renderer.swap();
	}

	glfwTerminate();
	return 0;
}
