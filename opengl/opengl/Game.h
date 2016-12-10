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
#include "Skybox.h"
#include "Lights.h"
#include "ResourceManager.h"
#include"ParticleSystem.h"


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
	bool IsPositionValid(std::tuple<int, int, int>, int typeOfObject);
	void DetectCollisions();
	void Shoot();

private:
	Model spaceShip;
	Model pointLightContainer;
	Model sphere;
	Model planet;
	
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
	ParticleSystem* coralParticle1;
	ParticleSystem* coralParticle2;
	ParticleSystem* coralParticle3;
	ParticleSystem* coralParticle4;
	ParticleSystem* coralParticle5;
	ParticleSystem* coralParticle6;
	ParticleSystem* coralParticle7;
	ParticleSystem* coralParticle8;
	ParticleSystem* coralParticle9;
	ParticleSystem* coralParticle10;
	//coral models
	Model coral1;
	Model coral2;
	Model coral3;
	Model coral4;
	Model coral5;
	Model coral6;
	Model coral7;
	Model coral8;
	Model coral9;
	Model coral10;

	//coral positions
	glm::vec3 coralPosition1 = glm::vec3(1, 12, 23.75);
	glm::vec3 coralPosition2 = glm::vec3(1, 15.5, 20.25);
	glm::vec3 coralPosition3 = glm::vec3(5.5, 15.25, 19.25);
	glm::vec3 coralPosition4 = glm::vec3(7, 18.5, 15.25);
	glm::vec3 coralPosition5 = glm::vec3(-1.75, 21, 15.25);
	glm::vec3 coralPosition6 = glm::vec3(-0.75, 22.25, 12.25);
	glm::vec3 coralPosition7 = glm::vec3(-1.75, 23.75, 9);
	glm::vec3 coralPosition8 = glm::vec3(-4.5, 23.25, 7.5);
	glm::vec3 coralPosition9 = glm::vec3(-2.25, 24, 4.5);
	glm::vec3 coralPosition10 = glm::vec3(2,24.25, 2);

};

#endif
