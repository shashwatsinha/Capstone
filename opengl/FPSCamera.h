#pragma once
#include <math.h>
#include <GL/glew.h>
#include <GLFW\glfw3.h>
#include <iostream>

#define M_PI 3.14f;
using namespace std;
class FPSCamera
{
private:

public:
	FPSCamera();

	void lockCamera();
	void moveCamera(float, float);
	void moveCameraUp(float, float);
	void control(float moveVel, float mouseVel, bool mi, GLFWwindow *ptr_window);
	float camX, camY, camZ, camYaw, camPitch;
	bool mouseIn;
	void updateCamera();
	float PI = 3.1415926535;
};