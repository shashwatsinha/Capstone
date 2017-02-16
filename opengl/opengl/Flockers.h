#pragma once
#include "BGMovingObjects.h" 
#include <ppl.h>
class Flockers :public BGMovingObjects
{
public:
	
	void InitializeFlock(float flockSize,glm::vec3 centre, float delay,bool seperate,float sDelay, bool patternMovement, int patternNumber);
	Flockers();
	~Flockers();
	
	void RenderFlock(Shader *shader);

private:
	vector<BGMovingObjects*> objs;
	glm::vec3 centreOfFlock;
	float seperator;
	float theta;

	GLfloat currentTime;
	GLfloat flockTime;

	float updateDelay;
	float seperatorTime;
	float seperatorDelay;

	void UpdateVelocity();		//Paramter here means how soon you want to update the velocity
	glm::vec3 ComputeAlignment(BGMovingObjects *obj);
	glm::vec3 ComputeCohesion(BGMovingObjects *obj, glm::vec3 centreOfFlock);
	glm::vec3 ComputeSeperation(BGMovingObjects *obj);
	glm::vec3 LimitFlockVelocity(glm::vec3 speed, float s);
	void ChangeCentreOfFlock(glm::vec3 centre);
	void ChangeCentreOfFlockLinearPositiveX();
	void ChangeCentreOfFlockCircular();

	bool patternTrue;
	int patternNumber;
	bool seperate;
};

