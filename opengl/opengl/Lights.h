#pragma once
// Std. Includes
#include <string>
#include <iostream>
#include <vector>
#include "Shader.h"
#include "Camera.h"
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


struct DirectionalLight {
	// Direction
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class Lights {
private:
	DirectionalLight light;
public:
	Lights(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
		light.direction = direction;
		light.ambient = ambient;
		light.diffuse = diffuse;
		light.specular = specular;
	}

	void setUpDirectionalLight(Shader *shader, Camera *camera) {
		// Pass view position to calculate specular light
		GLint viewPosLoc = glGetUniformLocation(shader->Program, "viewPos");
		glUniform3f(viewPosLoc, Camera::instance()->Position.x, Camera::instance()->Position.y, Camera::instance()->Position.z);

		// Set the Light Properties
		GLint lightDirPos = glGetUniformLocation(shader->Program, "directionalLight.direction");
		GLint lightAmbientLoc = glGetUniformLocation(shader->Program, "directionalLight.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(shader->Program, "directionalLight.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(shader->Program, "directionalLight.specular");
		glUniform3f(lightDirPos, light.direction.x, light.direction.x, light.direction.x);
		glUniform3f(lightAmbientLoc, light.ambient.x, light.ambient.x, light.ambient.x);
		glUniform3f(lightDiffuseLoc, light.diffuse.x, light.diffuse.x, light.diffuse.x);
		glUniform3f(lightSpecularLoc, light.specular.x, light.specular.x, light.specular.x);
	}
};