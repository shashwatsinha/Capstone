#include "BGMovingObjects.h"



BGMovingObjects::BGMovingObjects()
{
}


BGMovingObjects::~BGMovingObjects()
{
	this->velocity = velocity;
}

void BGMovingObjects::Update(Shader * shader)
{
	this->SetPosition(this->GetPosition() + velocity);

	if (this->GetPosition().x > 50.0f)
	{
		this->SetPosition(glm::vec3(-50.0f, this->GetPosition().y, this->GetPosition().z));
	}

	if (this->GetPosition().x < -50.0f)
	{
		this->SetPosition(glm::vec3(50.0f, this->GetPosition().y, this->GetPosition().z));
	}

	if (this->GetPosition().y > 50.0f)
	{
		this->SetPosition(glm::vec3(this->GetPosition().x, -50.0f, this->GetPosition().z));
	}

	if (this->GetPosition().y < -50.0f)
	{
		this->SetPosition(glm::vec3(this->GetPosition().x, 50.0f , this->GetPosition().z));
	}

	if (this->GetPosition().z > 50.0f)
	{
		this->SetPosition(glm::vec3(this->GetPosition().x, this->GetPosition().y, -50.0f));
	}

	if (this->GetPosition().z < -50.0f)
	{
		this->SetPosition(glm::vec3(this->GetPosition().x, this->GetPosition().y, 50.0f));
	}


	Draw(shader);
}

void BGMovingObjects::SetVelocity(glm::vec3 velocity)
{
	initialPosition = this->GetPosition();
	this->velocity = velocity;
}

void BGMovingObjects::ResetPosition()
{
	this->SetPosition(initialPosition);
}
