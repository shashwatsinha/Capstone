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
	velocityOfEnemy = glm::vec3(0, 0, 0);
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

void NormalEnemy::AddSphereCollider(float radius, glm::vec3 position)
{
	sphereCollider = new SphereCollider();
	sphereCollider->InitParameters(radius, position);
}

void NormalEnemy::UpdateCollider()
{
	sphereCollider->SetPosition(sphereCollider->GetPosition() + velocityOfEnemy);
	this->SetPosition(sphereCollider->GetPosition());
}

void NormalEnemy::SetVelocity(glm::vec3 velocity)
{
	velocityOfEnemy = velocity;
}

SphereCollider * NormalEnemy::GetCollider()
{
	return sphereCollider;
}

void NormalEnemy::SetHealth(int h)
{
	health = h;
}

int NormalEnemy::GetHealth()
{
	return health;
}

void NormalEnemy::ReduceHealth(int h)
{
	health = health - h;
}
