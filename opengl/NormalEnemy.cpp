#include "NormalEnemy.h"


float NormalEnemy::DistanceFromPlayer(glm::vec3 playerPos)
{
	float x = playerPos.x - enemyPosition.x;
	float y = playerPos.y - enemyPosition.y;
	float z = playerPos.z - enemyPosition.z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

void NormalEnemy::ProcessAI(glm::vec3 playerPos)
{
	playerPosition = playerPos;
	glm::vec3 k = GetPosition();
	enemyPosition = GetPosition();
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
	//std::cout << playerPosition.x << " " << playerPosition.y << " " << playerPosition.z << endl;
}

void NormalEnemy::Chasing()
{
	
	//glm::vec3 k = glm::vec3(DistanceVectorFromPlayer().x *2, DistanceVectorFromPlayer().y * 2, DistanceVectorFromPlayer().z *2);
	//SetPosition(k);
	//enemyPosition = k;
	//std::cout << "Chasing" <<" "<<enemyPosition.x<<" "<< enemyPosition.y<<" "<< enemyPosition.z<< endl;
}

void NormalEnemy::Attacking()
{
	//std::cout << "Attacking" << endl;
	//std::cout << enemyPosition.x << " " << enemyPosition.y << " " << enemyPosition.z << endl;
}

void NormalEnemy::ReduceHealth(int amount)
{
	health = health - amount;
}

int NormalEnemy::GetHeatlh()
{
	return health;
}

glm::vec3 NormalEnemy::DistanceVectorFromPlayer()
{
	
	return glm::normalize(playerPosition - enemyPosition);
}

void NormalEnemy::TestMove()
{
	glm::vec3 k = glm::vec3(GetPosition().x + 1, 0, 15);
	SetPosition(k);

}
