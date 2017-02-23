#pragma once
#include "PlaneClass.h"
class Frustum
{
public:
	Frustum();
	~Frustum();
	void ConstructFrustum(float screenDepth, glm::mat4 projectionMatrix, glm::mat4 viewMatrix);
	bool CheckSphere(glm::vec3 centre, float radius);
	
private:
	PlaneClass frustum[6];
};

