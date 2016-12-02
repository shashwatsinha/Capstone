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
	DirectionalLight dirLight;
public:
	Lights() 
	{

	}

	void initializeDirectionalLightParameters(glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
	{
		dirLight.direction = direction;
		dirLight.ambient = ambient;
		dirLight.diffuse = diffuse;
		dirLight.specular = specular;
	}

	void setUpDirectionalLight(Shader *shader, Camera *camera) {
		// Pass view position to calculate specular light
		GLint viewPosLoc = glGetUniformLocation(shader->ID, "viewPos");
		glUniform3f(viewPosLoc, Camera::instance()->Position.x, Camera::instance()->Position.y, Camera::instance()->Position.z);

		// Set the Light Properties
		GLint lightDirPos = glGetUniformLocation(shader->ID, "directionalLight.direction");
		GLint lightAmbientLoc = glGetUniformLocation(shader->ID, "directionalLight.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(shader->ID, "directionalLight.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(shader->ID, "directionalLight.specular");
		glUniform3f(lightDirPos, dirLight.direction.x, dirLight.direction.x, dirLight.direction.x);
		glUniform3f(lightAmbientLoc, dirLight.ambient.x, dirLight.ambient.x, dirLight.ambient.x);
		glUniform3f(lightDiffuseLoc, dirLight.diffuse.x, dirLight.diffuse.x, dirLight.diffuse.x);
		glUniform3f(lightSpecularLoc, dirLight.specular.x, dirLight.specular.x, dirLight.specular.x);
	}
};