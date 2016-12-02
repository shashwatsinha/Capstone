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
	void Render(GLfloat dt);

	// Other methods specific for our game
	void GenerateEnvironment();
	bool IsPositionValid(std::tuple<int, int, int>, int typeOfObject);
	void DetectCollisions();
	void Shoot();

private:
	Model spaceShip;
	Skybox skybox;
	Lights directionalLight;

	vector<NormalEnemy*> physicsObjects;
	vector<Bullet*> bullets;
	Bullet *masterBullet;
	// The MAIN function, from here we start our application and run our Game loop
	map<tuple<int, int, int>, int> mapPositions;

	float currentBulletFired = 0;
	float previousBulletFired = 0;
};

#endif
