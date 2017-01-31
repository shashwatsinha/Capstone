#pragma once
#include "Model.h"
#include <time.h> 
class EnvironmentObject : public Model
{
public:
	EnvironmentObject();
	~EnvironmentObject();
	void SetVelocity(glm::vec3 velocity);
	void Update(Shader *shader,glm::vec3 playerPos);
	float DistanceFromPlayer();
	glm::vec3 ReturnPerpendicularVector(glm::vec3 inputVector);
	bool IsVelocityZero();

private:
	glm::vec3 velocity;
	glm::vec3 playerPos;
};

