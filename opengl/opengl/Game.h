#pragma once
#ifndef GAME_H
#define GAME_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp> 
#include <glm/gtx/quaternion.hpp>
#include <SOIL.h>


#include "Camera.h"
#include "ActorFactory.h"
#include <stdlib.h>    
#include <time.h>
#include <map>
#include <tuple>
#include "Bullet.h"
#include "Coral.h"
#include "ParticleSystem.h"
#include "Skybox.h"
#include "Lights.h"
#include "BGMovingObjects.h"
#include "ResourceManager.h"
#include "Satellite.h"

// Include the Oculus SDK
#include "GL/CAPI_GLE.h"
#include "ThirdParty\OculusSDK\LibOVR\Include\Extras\OVR_Math.h"
#include "ThirdParty\OculusSDK\LibOVR\Include\OVR_CAPI_GL.h"

#if defined(_WIN32)
#include <dxgi.h> // for GetDefaultAdapterLuid
#pragma comment(lib, "dxgi.lib")
#endif


using namespace OVR;


// Represents the current state of the game
enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

// Game holds all game-related state and functionality.
// Combines all game-related data into a single class for
// easy access to each of the components and manageability.
class Game
{
public:
	// Game state
	GameState              State;
	GLboolean              Keys[1024];
	GLuint                 Width, Height;
	bool isVR = false;
	bool Running = true;
	GLFWwindow* window;
	string windowTitle ;
	GLfloat deltaTime = 0.0f;
	GLfloat lastFrame = 0.0f;
	bool retryCreate = true;

	// Constructor/Destructor
	Game(GLuint width, GLuint height);
	~Game();
	// Initialize game state (load all shaders/textures/levels)
	void Init();
	// GameLoop
	void ProcessInput(GLfloat dt);
	void Update(GLfloat dt);
	void Render();

	// Other methods specific for our game
	void GenerateEnvironment();
	ovrGraphicsLuid GetDefaultAdapterLuid();
	int Compare(const ovrGraphicsLuid & lhs, const ovrGraphicsLuid & rhs);
	bool RenderOculus();
	void RunVR();
	//void Run(bool(*MainLoop)(bool retryCreate));
	double calcFPS(double theTimeInterval, string theWindowTitle);
	bool IsPositionValid(std::tuple<int, int, int>, int typeOfObject);
	void DetectCollisions();
	void Shoot();
	float GetDeterminant(glm::vec3 k);
	glm::vec3 MultiplyVector(glm::vec3 a, float k);

	//Flocking Behaviour functions
	glm::vec3 ComputeAlignment(BGMovingObjects *obj, vector<BGMovingObjects*>objs);
	glm::vec3 ComputeCohesion(BGMovingObjects *obj, glm::vec3 centreOfFlock, vector<BGMovingObjects*>objs);
	glm::vec3 ComputeSeperation(BGMovingObjects *obj, vector<BGMovingObjects*>objs);
	glm::vec3 LimitFlockVelocity(glm::vec3 speed,  float s);
	void ChangeCentreOfFlock(glm::vec3 centre);


private:
	//Model spaceShip;
	Model *pointLightContainer;
	Model *sphere;
	Model *planet, *pinkPlanet;
	
	//Skybox skybox;
	Lights directionalLight;
	vector<Lights> pointLights;

	vector<NormalEnemy*> physicsObjects;
	vector<Bullet*> bullets;
	Bullet *masterBullet;
	// The MAIN function, from here we start our application and run our Game loop
	map<tuple<int, int, int>, int> mapPositions;

	float currentBulletFired = 0;
	float previousBulletFired = 0;

	// Point Light Distance array. Do not add distances manually. The pre initialized array is based on the point light properties.
	int pointLightDistance[9] = {7,13,20,32,50,65,100,160,200};

	GLuint texture1;
	ParticleSystem*  particlesystem1;
	ParticleSystem*  particlesystem2;
	//coral particlesystem

	//coral models
	
	vector<Coral* >corals;
	float mixValue;
	int noOfCoralsActivated = 0;
	vector<ParticleSystem*> coralParticles;

	EnvironmentObject bgObject;
	EnvironmentObject bgObject2;
	EnvironmentObject bgObject3;
	EnvironmentObject bgObject4;

	
	vector<BGMovingObjects*> movingObjs1;
	vector<BGMovingObjects*> movingObjs2;
	vector<BGMovingObjects*> movingObjs3;

	vector<Satellite*>satellites;

	BGMovingObjects *leader;
	glm::vec3 centreOfFlock1;
	glm::vec3 centreOfFlock3;
	GLfloat flock1CurTime;
	GLfloat flock2CurTime;
	glm::vec3 centreOfFlock2;
	GLfloat currentTime;

	float seperator;
};

#endif
