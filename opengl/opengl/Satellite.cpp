#include "Satellite.h"



Satellite::Satellite(int c)
{
	theta = 0;
	config = c;
}


Satellite::~Satellite()
{
}

void Satellite::Update(Shader * shader)
{
	switch (config)
	{
	case 1:
		this->SetPosition(glm::vec3(30 * sin(theta), 30 * cos(theta), this->GetPosition().z));
		break;
	case 2:
		this->SetPosition(glm::vec3(30 * sin(theta), this->GetPosition().y, 30 * cos(theta) ));
		break;
	case 3:
		this->SetPosition(glm::vec3(this->GetPosition().x, 30 * sin(theta), 30 * cos(theta)));
		break;
	case 4:
		this->SetPosition(glm::vec3(30 * sin(theta), 30 * cos(theta), 30 * cos(theta)));
		break;
	case 5:
		this->SetPosition(glm::vec3(30 * sin(theta), 30 * cos(theta), 30 * sin(theta)));
		break;
	case 6:
		this->SetPosition(glm::vec3(30 * cos(theta), 30 * cos(theta), 30 * sin(theta)));
		break;

	}
	
	theta = theta + 0.01f;
	Draw(shader);
}
