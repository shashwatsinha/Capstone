#pragma once
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include "Model.h"

class Coral : public Model
{
public:
	Coral();
	~Coral();

	float DistanceFromPlayer(glm::vec3 playerPos);

	void IsParticleActivated(glm::vec3 pos);

	bool ActivateParticles();

private:
	bool emitParticles;
	glm::vec3 playerPos;
};

