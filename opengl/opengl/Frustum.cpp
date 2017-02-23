#include "Frustum.h"



Frustum::Frustum()
{
}


Frustum::~Frustum()
{
}

void Frustum::ConstructFrustum(float screenDepth, glm::mat4 projectionMatrix, glm::mat4 viewMatrix)
{
	float zMinimum, r;
	glm::mat4 matrix;


	// Calculate the minimum Z distance in the frustum.
	zMinimum = -projectionMatrix[3][2] / projectionMatrix[2][2];
	r = screenDepth / (screenDepth - zMinimum);
	projectionMatrix[2][2] = r;
	projectionMatrix[3][2] = -r * zMinimum;
	matrix = glm::matrixCompMult(viewMatrix, projectionMatrix);
	// Create the frustum matrix from the view matrix and updated projection matrix.

	// Calculate near plane of frustum.
	frustum[0].a = matrix[0][3] + matrix[0][2];
	frustum[0].b = matrix[1][3] + matrix[1][2];
	frustum[0].c = matrix[2][3] + matrix[2][2];
	frustum[0].d = matrix[3][3] + matrix[3][2];
	frustum[0] = frustum[0].NormalizePlane();

	// Calculate far plane of frustum.
	frustum[1].a = matrix[0][3] - matrix[0][2];
	frustum[1].b = matrix[1][3] - matrix[1][2];
	frustum[1].c = matrix[2][3] - matrix[2][2];
	frustum[1].d = matrix[3][3] - matrix[3][2];
	frustum[1] = frustum[1].NormalizePlane();

	// Calculate left plane of frustum.
	frustum[2].a = matrix[0][3] + matrix[0][0];
	frustum[2].b = matrix[1][3] + matrix[1][0];
	frustum[2].c = matrix[2][3] + matrix[2][0];
	frustum[2].d = matrix[3][3] + matrix[3][0];
	frustum[2] = frustum[2].NormalizePlane();

	// Calculate right plane of frustum.
	frustum[3].a = matrix[0][3] - matrix[0][0];
	frustum[3].b = matrix[1][3] - matrix[1][0];
	frustum[3].c = matrix[2][3] - matrix[2][0];
	frustum[3].d = matrix[3][3] - matrix[3][0];
	frustum[3] = frustum[3].NormalizePlane();

	// Calculate top plane of frustum.
	frustum[4].a = matrix[0][3] - matrix[0][1];
	frustum[4].b = matrix[1][3] - matrix[1][1];
	frustum[4].c = matrix[2][3] - matrix[2][1];
	frustum[4].d = matrix[3][3] - matrix[3][1];
	frustum[4] = frustum[4].NormalizePlane();

	// Calculate bottom plane of frustum.
	frustum[5].a = matrix[0][3] + matrix[0][1];
	frustum[5].b = matrix[1][3] + matrix[1][1];
	frustum[5].c = matrix[2][3] + matrix[2][1];
	frustum[5].d = matrix[3][3] + matrix[3][1];
	frustum[5] = frustum[0].NormalizePlane();
}

bool Frustum::CheckSphere(glm::vec3 centre, float radius)
{
	for (int i = 0; i < 6; i++)
	{
		if (frustum[i].DotCoord(centre) < -radius)
		{
			return false;
		}
	}

	return true;
}
