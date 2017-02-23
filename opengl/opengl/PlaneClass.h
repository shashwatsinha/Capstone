#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
class PlaneClass
{
public:
	PlaneClass();
	~PlaneClass();
	void ConstructPlane(float p1, float p2, float p3, float p4);
	PlaneClass NormalizePlane();
	float DotCoord(glm::vec3 point);
	float a;
	float b;
	float c;
	float d;
private:
	
};

