#include "Bullet.h"



Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}

void Bullet::AddSphereCollider(float radius, glm::vec3 position)
{
	sphereCollider = new SphereCollider();
	sphereCollider->InitParameters(radius, position);
}

void Bullet::UpdateCollider()
{
	sphereCollider->SetPosition(sphereCollider->GetPosition() + velocityOfBullet);
	this->SetPosition(sphereCollider->GetPosition());
}

void Bullet::SetVelocity(glm::vec3 velocity)
{
	glm::vec3 vel = glm::vec3(velocity.x / 100, velocity.y / 100, velocity.z / 100);
	velocityOfBullet = vel;

}

SphereCollider * Bullet::GetCollider()
{
	return sphereCollider;
}
