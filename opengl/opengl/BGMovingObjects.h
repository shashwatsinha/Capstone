#pragma once

#include "Model.h"
#include <time.h> 

class BGMovingObjects : public Model
{
public:
	BGMovingObjects();
	~BGMovingObjects();
	void Update(Shader *shader);
	void SetVelocity(glm::vec3 velocity);
	void ResetPosition();
private:
	glm::vec3 velocity;
	glm::vec3 initialPosition;
};

