#pragma once

#include "Model.h"
#include <time.h> 
#include <GLFW/glfw3.h>

class BGMovingObjects : public Model
{
public:
	BGMovingObjects();
	~BGMovingObjects();
	void Update(Shader *shader);
	void SetVelocity(glm::vec3 velocity);
	void ResetPosition();
	void ChangeVelocity(float ct);
	float DistanceFrom(glm::vec3 k);
	glm::vec3 GetVelocity();
	
	

private:
	glm::vec3 velocity;
	glm::vec3 initialPosition;
	GLfloat currentTime;
};

