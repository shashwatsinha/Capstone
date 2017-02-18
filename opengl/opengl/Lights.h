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


struct DirLight {
	// Direction
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight {
	glm::vec3 position;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	int distance;
};

class Lights {
private:
	DirLight dirLight;
	PointLight pointLight;
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

	void setPointLightParameters(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, int distance)
	{
		pointLight.position = position;
		pointLight.ambient = ambient;
		pointLight.diffuse = diffuse;
		pointLight.specular = specular;
		setPointLightAttenuationProperties(distance);
	}

	void setUpDirectionalLight(Shader *shader, Camera *camera) {
		// Pass view position to calculate specular light
		GLint viewPosLoc = glGetUniformLocation(shader->ID, "viewPos");
		glUniform3f(viewPosLoc, Camera::instance()->Position.x, Camera::instance()->Position.y, Camera::instance()->Position.z);

		// Set the Light Properties
		GLint lightDirPos = glGetUniformLocation(shader->ID, "dirLight.direction");
		GLint lightAmbientLoc = glGetUniformLocation(shader->ID, "dirLight.ambient");
		GLint lightDiffuseLoc = glGetUniformLocation(shader->ID, "dirLight.diffuse");
		GLint lightSpecularLoc = glGetUniformLocation(shader->ID, "dirLight.specular");
		glUniform3f(lightDirPos, dirLight.direction.x, dirLight.direction.y, dirLight.direction.z);
		glUniform3f(lightAmbientLoc, dirLight.ambient.x, dirLight.ambient.y, dirLight.ambient.z);
		glUniform3f(lightDiffuseLoc, dirLight.diffuse.x, dirLight.diffuse.y, dirLight.diffuse.z);
		glUniform3f(lightSpecularLoc, dirLight.specular.x, dirLight.specular.y, dirLight.specular.z);
	}

	void setUpPointLight(Shader *shader, Camera *camera, string *uniformName) {
		// Pass view position to calculate specular light
		GLint viewPosLoc = glGetUniformLocation(shader->ID, "viewPos");
		glUniform3f(viewPosLoc, Camera::instance()->Position.x, Camera::instance()->Position.y, Camera::instance()->Position.z);

		// Set the Light Properties
		GLint lightPosLoc = glGetUniformLocation(shader->ID, uniformName[0].c_str());
		GLint lightAmbientLoc = glGetUniformLocation(shader->ID, uniformName[1].c_str());
		GLint lightDiffuseLoc = glGetUniformLocation(shader->ID, uniformName[2].c_str());
		GLint lightSpecularLoc = glGetUniformLocation(shader->ID, uniformName[3].c_str());
		GLint lightConstantLoc = glGetUniformLocation(shader->ID, uniformName[4].c_str());
		GLint lightLinearLoc = glGetUniformLocation(shader->ID, uniformName[5].c_str());
		GLint lightQuadraticLoc = glGetUniformLocation(shader->ID, uniformName[6].c_str());
		glUniform3f(lightPosLoc, pointLight.position.x, pointLight.position.y, pointLight.position.z);
		glUniform3f(lightAmbientLoc, pointLight.ambient.x, pointLight.ambient.y, pointLight.ambient.z);
		glUniform3f(lightDiffuseLoc, pointLight.diffuse.x, pointLight.diffuse.y, pointLight.diffuse.z);
		glUniform3f(lightSpecularLoc, pointLight.specular.x, pointLight.specular.y, pointLight.specular.z);
		glUniform1f(lightConstantLoc, pointLight.constant);
		glUniform1f(lightLinearLoc, pointLight.linear);
		glUniform1f(lightQuadraticLoc, pointLight.quadratic);
	}

	void setNoOfPointLights(Shader *shader, int noOfPointLights)
	{
		GLint lightNoOfLightsLoc = glGetUniformLocation(shader->ID, "NO_OF_POINT_LIGHTS");
		glUniform1i(lightNoOfLightsLoc, noOfPointLights);
	}

	void setPointLightAttenuationProperties(int distance)
	{
		pointLight.constant = 1.0f;
		switch (distance)
		{
			case 7:
				pointLight.linear = 0.7f;
				pointLight.quadratic = 1.8f;
				break;
			case 13:
				pointLight.linear = 0.35f;
				pointLight.quadratic = 0.44f;
				break;
			case 20:
				pointLight.linear = 0.22f;
				pointLight.quadratic = 0.2f;
				break;
			case 32:
				pointLight.linear = 0.14f;
				pointLight.quadratic = 0.07f;
				break;
			case 50:
				pointLight.linear = 0.09f;
				pointLight.quadratic = 0.032f;
				break;
			case 65:
				pointLight.linear = 0.07f;
				pointLight.quadratic = 0.017f;
				break;
			case 100:
				pointLight.linear = 0.045f;
				pointLight.quadratic = 0.0075f;
				break;
			case 160:
				pointLight.linear = 0.027f;
				pointLight.quadratic = 0.0028f;
				break;
			case 200:
				pointLight.linear = 0.022f;
				pointLight.quadratic = 0.0019f;
				break;
		}
	}
};