#include "Flockers.h"



glm::vec3 Flockers::ComputeAlignment(BGMovingObjects * obj)
{
	glm::vec3 point;
	concurrency::parallel_for(size_t(0), objs.size(), [&](size_t i)
	{
		if (objs[i] != obj)
		{
			{
				point += objs[i]->GetVelocity();
			}
		}
	});


	point.x /= objs.size();	point.y /= objs.size();	point.z /= objs.size();

	point = glm::vec3((point.x - obj->GetVelocity().x) / 8.0f,
		(point.y - obj->GetVelocity().y) / 8.0f,
		(point.z - obj->GetVelocity().z) / 8.0f);

	return point;
}

glm::vec3 Flockers::ComputeCohesion(BGMovingObjects * obj, glm::vec3 centreOfFlock)
{
	glm::vec3 centre = centreOfFlock;

	concurrency::parallel_for(size_t(0), objs.size(), [&](size_t i)
	{
		if (objs[i] != obj)
		{
			{
				centre += objs[i]->GetPosition();

			}
		}
	});

	centre.x /= objs.size();	centre.y /= objs.size();	centre.z /= objs.size();
	centre = glm::vec3((centre.x - obj->GetPosition().x) / 100,
		(centre.y - obj->GetPosition().y) / 100,
		(centre.z - obj->GetPosition().z) / 100);

	return centre;
}

glm::vec3 Flockers::ComputeSeperation(BGMovingObjects * obj)
{
	glm::vec3 point;

	concurrency::parallel_for(size_t(0), objs.size(), [&](size_t i)
	{
		if (objs[i] != obj)
		{
			if (obj->DistanceFrom(objs[i]->GetPosition()) < 100)
			{
				point = point - (objs[i]->GetPosition() - obj->GetPosition());
			}
		}
	});

	return point;
}

glm::vec3 Flockers::LimitFlockVelocity(glm::vec3 speed, float s)
{
	float vLim = s;
	glm::vec3 v = speed;

	if (GetDeterminant(speed) > vLim)
	{
		v /= GetDeterminant(v);
		v *= vLim;
	}

	return v;
}

void Flockers::ChangeCentreOfFlock(glm::vec3 centre)
{
	
}

void Flockers::ChangeCentreOfFlockLinearPositiveX()
{
	centreOfFlock.x += 0.01f;
}

void Flockers::ChangeCentreOfFlockCircular()
{
	centreOfFlock = glm::vec3(50 * sin(theta), 50 * cos(theta), centreOfFlock.z);
	theta = theta + 0.01f;
	if (theta > 360.0f)
		theta = 0.0f;
}

void Flockers::InitializeFlock(float flockSize, glm::vec3 centre, float delay,  bool s, float sDelay, bool patternMovement, int number)
{
	for (int i = 0; i < flockSize; i++)
	{
		BGMovingObjects *temp = new BGMovingObjects();
		temp->InitPath("Models/BlueObject/BlueObject.obj");
		temp->SetScale(glm::vec3(0.03f, 0.03f, 0.03f));
		temp->SetPosition(centre);
		temp->SetVelocity(glm::vec3(0.0f, 0.1f, -0.1f));
		objs.push_back(temp);		
	}

	centreOfFlock = centre;
	seperator = 1;
	this->updateDelay = delay;
	seperatorDelay = sDelay;
	seperatorTime = 0.0f;
	flockTime = 0.0f;
	patternTrue = patternMovement;
	patternNumber = number;
	theta = 0.0f;
	seperate = s;
}

Flockers::Flockers()
{
}


Flockers::~Flockers()
{
}

void Flockers::RenderFlock(Shader * shader)
{
	for (int i = 0; i < objs.size(); i++)
	{
		objs[i]->Update(shader);
	}

	currentTime = glfwGetTime();

	if (currentTime - flockTime > updateDelay)
	{
		UpdateVelocity();
		flockTime = currentTime;
	}

	if (currentTime - seperatorTime > seperatorDelay && seperate==true)
	{
		seperator *= -1;
		seperatorTime = currentTime;
	}

	if (patternTrue)
	{
		switch (patternNumber)
		{
		case 1:
			ChangeCentreOfFlockCircular();
			break;
		case 2:
			ChangeCentreOfFlockLinearPositiveX();
			break;
		}
	}
}

void Flockers::UpdateVelocity()
{

	for (int i = 0; i < objs.size(); i++)
	{

		glm::vec3 temp = objs[i]->GetVelocity() +
			ComputeAlignment(objs[i]) +
			MultiplyVector(ComputeCohesion(objs[i], centreOfFlock), seperator) +
			ComputeSeperation(objs[i]);

		temp = LimitFlockVelocity(temp, 0.1f);
		objs[i]->SetVelocity(temp);
	}
}


