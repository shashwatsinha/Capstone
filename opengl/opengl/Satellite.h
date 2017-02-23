#pragma once

#include "Model.h"
#include <time.h> 
#include <GLFW/glfw3.h>


class Satellite : public Model
{
public:
	Satellite(int c);
	~Satellite();
	void Update(Shader *shader);
	float GetRadius();
	
private:
	float theta;
	int config;
	float radius;
};

