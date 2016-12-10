#include "Coral.h"



Coral::Coral()
{
	lerpColor = false;
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
	if (DistanceFromPlayer(playerPos) > 200)
	{
		emitParticles = false;
	}

	else
	{
		emitParticles = true;
		lerpColor = true;
	}
}

bool Coral::ActivateParticles()
{
	return emitParticles;
}

void Coral::SetLerpColorStatus(bool status)
{
	lerpColor = status;
}

bool Coral::GetLerpColorStatus()
{
	return lerpColor;
}
