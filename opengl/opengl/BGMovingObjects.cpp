#include "BGMovingObjects.h"



BGMovingObjects::BGMovingObjects()
{
	currentTime = 0.0f;
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

	GLfloat ct = glfwGetTime();
	if (ct - currentTime > 5.0f)
	{
		ChangeVelocity(ct);
		currentTime = ct;
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

void BGMovingObjects::ChangeVelocity(float ct)
{
	//srand(time(NULL));
	float a =   ((rand() % 10 - 5.0f) / 500.0f); // this->velocity.x;
	float b =   ((rand() % 10 - 5) / 500.0f);     //   this->velocity.y;   
	float c = ((rand() % 5 - 10) / 100.0f);
	this->velocity = glm::vec3(a, b, c);
}
