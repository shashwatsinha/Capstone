#include "Coral.h"



Coral::Coral()
{
}


Coral::~Coral()
{
}

float Coral::DistanceFromPlayer(glm::vec3 playerPos)
{
	float x = playerPos.x - this->GetPosition().x;
	float y = playerPos.y - this->GetPosition().y;
	float z = playerPos.z - this->GetPosition().z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

void Coral::IsParticleActivated(glm::vec3 pos)
{
	playerPos = pos;
	if (DistanceFromPlayer(playerPos) > 2000)
	{
		emitParticles = false;
	}

	else
		emitParticles = true;
}

bool Coral::ActivateParticles()
{
	return emitParticles;
}
