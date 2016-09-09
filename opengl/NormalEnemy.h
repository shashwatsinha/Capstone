#pragma once
#include "Model.h"
class NormalEnemy :public Model
{
public:

	float DistanceFromPlayer(glm::vec3 playerPos);
	void ProcessAI();
	NormalEnemy();
	~NormalEnemy();
	enum State { patrol, chase, attack };

private:
	State currentState = patrol;
};

