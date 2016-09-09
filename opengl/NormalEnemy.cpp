#include "NormalEnemy.h"


float NormalEnemy::DistanceFromPlayer(glm::vec3 playerPos)
{
	float x = playerPos.x - this->GetPosition().x;
	float y = playerPos.y - this->GetPosition().y;
	float z = playerPos.z - this->GetPosition().z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

void NormalEnemy::ProcessAI()
{

}

NormalEnemy::NormalEnemy()
{
}


NormalEnemy::~NormalEnemy()
{
}
