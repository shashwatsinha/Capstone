#include "BGMovingObjects.h"



BGMovingObjects::BGMovingObjects()
{
	currentTime = 0.0f;
	initialPosition = this->GetPosition();
	
}


BGMovingObjects::~BGMovingObjects()
{
	
}

void BGMovingObjects::Update(Shader * shader)
{
	this->SetPosition(this->GetPosition() + velocity);
	/*
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
	}*/

	Draw(shader);
}

void BGMovingObjects::SetVelocity(glm::vec3 velocity)
{
	
	this->velocity = velocity;
}

void BGMovingObjects::ResetPosition()
{
	this->SetPosition(initialPosition);
}

void BGMovingObjects::ChangeVelocity(float ct)
{
	//srand(time(NULL));
	float a =    ((rand() % 10 ) / 1000.0f); // this->velocity.x;
	float b =   ((rand() % 10 ) / 2000.0f);     //   this->velocity.y;   
	float c =    this->velocity.z; //((rand() % 5 - 10) / 100.0f);   
	this->velocity = glm::vec3(a, b, c);

	//this->SetPosition ( glm::vec3( 30 * sin(x) , this->velocity.y,  30 * cos(x)));
	
}

float BGMovingObjects::DistanceFrom(glm::vec3 k)
{
	//float k = glm::distance(k, this->GetPosition());
	return 0;
}

glm::vec3 BGMovingObjects::GetVelocity()
{
	return velocity;
}
