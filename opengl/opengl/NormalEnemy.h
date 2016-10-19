#pragma once
#include "Model.h"
#include "SphereCollider.h"

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
	void AddSphereCollider(float radius,glm::vec3 position);
	void UpdateCollider();
	void SetVelocity(glm::vec3 velocity);
	SphereCollider* GetCollider();
	void SetHealth(int h);
	int GetHealth();
	void ReduceHealth(int h);

private:
	State currentState = Patrol;
	State previousState = Patrol;
	SphereCollider *sphereCollider;
	glm::vec3 velocityOfEnemy;
	int health;
};

