#pragma once
#include "Model.h"
#include "SphereCollider.h"
class Bullet:public Model
{
public:
	Bullet();
	~Bullet();
	void AddSphereCollider(float radius, glm::vec3 position);
	void UpdateCollider();
	void SetVelocity(glm::vec3 velocity);
	SphereCollider* GetCollider();
private:
	SphereCollider *sphereCollider;
	glm::vec3 velocityOfBullet;
};

