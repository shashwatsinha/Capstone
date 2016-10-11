#pragma once
#include "Model.h"
class NormalEnemy :public Model
{
public:

	float DistanceFromPlayer(glm::vec3 playerPos);
	void ProcessAI(glm::vec3 playerPos);
	NormalEnemy();
	~NormalEnemy();
	enum State { Patrol, Chase, Attack };
	void Patrolling();
	void Chasing();
	void Attacking();

	void ReduceHealth(int amount);
	int GetHeatlh();

private:
	State currentState = Patrol;
	State previousState = Patrol;
	int health;
};

