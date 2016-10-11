#include "NormalEnemy.h"


float NormalEnemy::DistanceFromPlayer(glm::vec3 playerPos)
{
	float x = playerPos.x - this->GetPosition().x;
	float y = playerPos.y - this->GetPosition().y;
	float z = playerPos.z - this->GetPosition().z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

void NormalEnemy::ProcessAI(glm::vec3 playerPos)
{
	if (DistanceFromPlayer(playerPos) < 25.0f && DistanceFromPlayer(playerPos) > 20.0f &&(currentState==Patrol || currentState==Attack))
	{
		currentState = Chase;
	}

	else if (DistanceFromPlayer(playerPos) < 20.0f && currentState == Chase)
	{
		currentState = Attack;
	}

	else if (DistanceFromPlayer(playerPos) > 25.0f)
	{
		currentState = Patrol;
	}

	switch (currentState)
	{
	case Attack:
		Attacking();
		break;
	case Chase:
		Chasing();
		break;
	case Patrol:
		Patrolling();
		break;
	}
}

NormalEnemy::NormalEnemy()
{
	health = 100;
}


NormalEnemy::~NormalEnemy()
{
}

void NormalEnemy::Patrolling()
{
	//std::cout << "Patrolling" << endl;
}

void NormalEnemy::Chasing()
{
	//std::cout << "Chasing" << endl;
}

void NormalEnemy::Attacking()
{
	//std::cout << "Attacking" << endl;
}

void NormalEnemy::ReduceHealth(int amount)
{
	health = health - amount;
}

int NormalEnemy::GetHeatlh()
{
	return health;
}
