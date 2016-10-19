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
	positionOfPlayer = playerPos;
	if (DistanceFromPlayer(playerPos) < 50.0f && DistanceFromPlayer(playerPos) > 30.0f &&(currentState==Patrol || currentState==Attack))
	{
		currentState = Chase;
	}

	else if (DistanceFromPlayer(playerPos) < 30.0f && currentState == Chase)
	{
		currentState = Attack;
	}

	else if (DistanceFromPlayer(playerPos) > 50.0f)
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
	std::cout << "Patrolling" << endl;
	SetVelocity(glm::vec3(0, 0, 0));
}

void NormalEnemy::Chasing()
{
	std::cout << "Chasing" << endl;
	glm::vec3 speedDirection = glm::normalize(positionOfPlayer - this->GetPosition());
	float speed = 0.01f;
	speedDirection = glm::vec3(speedDirection * speed);
	SetVelocity(speedDirection);
}

void NormalEnemy::Attacking()
{
	std::cout << "Attacking" << endl;
	SetVelocity(glm::vec3(0, 0, 0));
	currentBulletFired = glfwGetTime();
	float shootInterval = currentBulletFired - previousBulletFired;
	if (shootInterval > 0.01f)
	{
		Shoot();
	}
	previousBulletFired = currentBulletFired;
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

void NormalEnemy::Shoot()
{
	Bullet *enemy = new Bullet();
	enemy->InitPath("Models/Bullet/Bullet.obj");
	glm::vec3 shootPosition = glm::vec3(this->GetPosition());
	enemy->SetPosition(shootPosition);
	enemy->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	enemy->AddSphereCollider(2.0f, enemy->GetPosition());
	glm::vec3 bulletDirection = glm::normalize(positionOfPlayer - shootPosition);
	float bulletSpeed = 4.0f;
	bulletDirection = glm::vec3(bulletDirection * bulletSpeed);
	enemy->SetVelocity(bulletDirection);
	enemyBullets.push_back(enemy);
}
