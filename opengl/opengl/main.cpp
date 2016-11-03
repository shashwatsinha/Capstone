#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
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

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "assimp-vc130-mt.lib")

#include <iostream>

GLuint screenWidth = 800, screenHeight = 600;
int sizeA = 1;
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void GenerateEnvironment();
void InitGLFW();
bool IsPositionValid(std::tuple<int, int, int>, int typeOfObject);
void DetectCollisions();
void Shoot();
void SetViewAndProjection(Shader shader, glm::mat4 view);
double calcFPS(double theTimeInterval, string theWindowTitle);

// Camera
//Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLFWwindow* window;
vector<NormalEnemy*> physicsObjects;
vector<Bullet*> bullets;
Bullet *masterBullet;
// The MAIN function, from here we start our application and run our Game loop
map<tuple<int,int,int>, int> mapPositions;

float currentBulletFired = 0;
float previousBulletFired = 0;

string windowTitle = "Space Game";

int main()
{
	// Init GLFW
	InitGLFW();
	masterBullet = new Bullet();
	masterBullet->InitPath("Models/Bullet/Bullet.obj");
	// Setup and compile our shaders
	Shader shader("Shaders/vertexShader_default.vs", "Shaders/fragmentShader_default.frag");
	Shader skyboxShader("Shaders/vertexShader_Skybox.vs", "Shaders/fragmentShader_Skybox.frag");
	Camera::instance()->InitValues();
	srand(time(NULL));

	Skybox skybox;
	skybox.setupMesh();

	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	Lights directionalLight(direction, ambient, diffuse, specular);

	GenerateEnvironment();

	//Model ourModel;
	//ourModel.InitPath("Models/SpaceCraft/Wraith Raider Starship.obj");

	while (!glfwWindowShouldClose(window))
	{
		calcFPS(1.0, windowTitle);
		
		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		shader.Use();   // <-- Don't forget this one!
						// Transformation matrices
		glm::mat4 view = Camera::instance()->GetViewMatrix();
		SetViewAndProjection(shader, view);
		
		// Set Light properties
		directionalLight.setUpDirectionalLight(&shader, Camera::instance());

		for (int i = 0; i < physicsObjects.size(); i++)
		{
			physicsObjects[i]->ProcessAI(Camera::instance()->Position);
			physicsObjects[i]->UpdateCollider(deltaTime);
			physicsObjects[i]->Draw(&shader);
			for (int j = 0; j < physicsObjects[i]->enemyBullets.size(); j++)
			{
				physicsObjects[i]->enemyBullets[j]->UpdateCollider(deltaTime);
				physicsObjects[i]->enemyBullets[j]->Draw(&shader);
			}
		}

		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i]->UpdateCollider(deltaTime);
			bullets[i]->Draw(&shader);
		}

		// Do not delete the below commented
		// This is how you set the quarternion rotation
		/*
		ourModel.SetPosition(glm::vec3(0.0f, -1.75f, -2.0f));
		ourModel.SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
		glm::quat myQuat;
		glm::quat key_quat = glm::quat(glm::vec3(0.0f, (GLfloat)glfwGetTime() * glm::radians(20.0f), 0.0f));
		myQuat = key_quat * myQuat;
		myQuat = glm::normalize(myQuat);
		ourModel.SetRotation(myQuat);
		ourModel.Draw(&shader);
		*/

		// Draw skybox last
		glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(Camera::instance()->GetViewMatrix()));	// Remove any translation component of the view matrix
		SetViewAndProjection(skyboxShader, view);
		skybox.Draw(&skyboxShader);
		glDepthFunc(GL_LESS); // Set depth function back to default

		DetectCollisions();
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	delete masterBullet;
	glfwTerminate();
	return 0;
}

void SetViewAndProjection(Shader shader, glm::mat4 view) {
	glm::mat4 projection = glm::perspective(Camera::instance()->Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}



#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		Camera::instance()->ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		Camera::instance()->ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		Camera::instance()->ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		Camera::instance()->ProcessKeyboard(RIGHT, deltaTime);
	
	if (keys[GLFW_KEY_SPACE])
	{
		currentBulletFired = glfwGetTime();
		float shootInterval = currentBulletFired - previousBulletFired;
		if (shootInterval > 0.1f)
		{
			Shoot();
		}

		previousBulletFired = currentBulletFired;
	}
	
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	Camera::instance()->ProcessMouseMovement(xoffset, yoffset,true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::instance()->ProcessMouseScroll(yoffset);
}

#pragma endregion

void InitGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

}

//typeOfObject : 2 means its enemy, 3 means its an obstacle
bool IsPositionValid(std::tuple<int,int,int> positionTuple, int typeOfObject)
{
	bool flag = false;
	int minDistanceBetweenEnemies = 25;
	int minDistanceBetweenObstacles = 25;
	int minDistanceBetweenOE = 15;

	if (mapPositions[positionTuple] == NULL)
	{
		flag = true;
		for (auto it = mapPositions.begin(); it != mapPositions.end(); it++)
		{
			int x1 = get<0>(it->first); int x2 = get<0>(positionTuple);
			int y1 = get<1>(it->first); int y2 = get<1>(positionTuple);
			int z1 = get<2>(it->first); int z2 = get<2>(positionTuple);

			int d1 = pow(x1 - x2, 2);
			int d2 = pow(y1 - y2, 2);
			int d3 = pow(z1 - z2, 2);
			int value = it->second;
			switch (typeOfObject)
			{
			case 2: 
				if (value == 2)
				{
					if ((d1 + d2 + d3) < minDistanceBetweenEnemies && (d1 + d2 + d3)>0)
					{
						mapPositions.erase(positionTuple);
						flag = false;
						return flag;
					}
				}

				else if (value == 3)
				{
					if ((d1 + d2 + d3) < minDistanceBetweenOE && (d1 + d2 + d3)>0)
					{
						mapPositions.erase(positionTuple);
						flag = false;
						return flag;
					}
				}
				break;

			case 3:
				if (value == 2)
				{
					if ((d1 + d2 + d3) < minDistanceBetweenOE && (d1 + d2 + d3)>0)
					{
						mapPositions.erase(positionTuple);
						flag = false;
						return flag;
					}
				}

				else if (value == 3)
				{
					if ((d1 + d2 + d3) < minDistanceBetweenObstacles && (d1 + d2 + d3)>0)
					{
						mapPositions.erase(positionTuple);
						flag = false;
						return flag;
					}
				}
				break;

			}
		}
		return flag;
	}
	mapPositions.erase(positionTuple);
	return flag;
}

void DetectCollisions()
{
	//Check for collision between bullets by player and enemy objects
	for (int i = 0; i < physicsObjects.size(); )
	{
		bool delObj = false;
		for (int j = 0; j < bullets.size(); )
		{
			glm::vec3 distance = physicsObjects[i]->GetPosition() - bullets[j]->GetPosition();
			float sumOfRadii = physicsObjects[i]->GetCollider()->GetRadius() + bullets[j]->GetCollider()->GetRadius();
			float length = (distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z);
			
			//Destroy the bullet
			if (length < (sumOfRadii * sumOfRadii))
			{
				//Clean way to destroy the bullet
				Bullet *bullet = bullets[j];
				bullets.erase(bullets.begin() + j);
				delete bullet;
				physicsObjects[i]->ReduceHealth(10);
			}

			else if (bullets[j]->GetPosition().x > 100 || bullets[j]->GetPosition().x < -100 ||
				     bullets[j]->GetPosition().y > 100 || bullets[j]->GetPosition().y < -100 ||
					 bullets[j]->GetPosition().z > 100 || bullets[j]->GetPosition().z < -100)
			{
				Bullet *bullet = bullets[j];
				bullets.erase(bullets.begin() + j);
				delete bullet;
				physicsObjects[i]->ReduceHealth(10);
			}

			else
			{
				++j;
			}
		}

		//Destroy the enemy
		if (physicsObjects[i]->GetHealth() <= 0)
		{
			delObj = true;
			NormalEnemy *enemy = physicsObjects[i];
			physicsObjects.erase(physicsObjects.begin() + i);
			delete enemy;
		}
		else
		{
			++i;
		}
	}
}

void Shoot()
{
	Bullet *enemy = new Bullet();
	*enemy = *masterBullet;
	glm::vec3 shootPosition = glm::vec3(Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y - 1.0f, Camera::instance()->GetPosition().z);
	enemy->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	enemy->AddSphereCollider(2.0f, enemy->GetPosition());
	glm::vec3 bulletDirection = glm::normalize(Camera::instance()->Front);
	float bulletSpeed = 4.0f;
	bulletDirection = glm::vec3(bulletDirection * bulletSpeed);
	enemy->SetValues(shootPosition, bulletDirection);
	bullets.push_back(enemy);
}

void GenerateEnvironment()
{
	ifstream inputFile;
	inputFile.open("EnvironmentCoordinates/TrainedValues.txt");
	vector< vector<int> > coordinates;

	NormalEnemy *masterEnemy = new NormalEnemy();
	masterEnemy->InitPath("Models/SpaceCraft/Wraith Raider Starship.obj");
	masterEnemy->SetType(1);

	NormalEnemy *masterAsteroid = new NormalEnemy();
	masterAsteroid->InitPath("Models/Asteroid/Asteroid.obj");
	masterAsteroid->SetType(0);

	for (int i = 0; i < 23; i++)
	{
		vector<int> temp;
		char tempString = NULL;
		for (int j = 0; j < 40; j++)
		{
			inputFile >> tempString;
			int input = tempString - '0';
			if (input == 1)
			{
				NormalEnemy *enemy = new NormalEnemy();
				*enemy = *masterEnemy;
				enemy->SetValues(glm::vec3(i*5, (rand() % 5 - 5), j*-10), glm::vec3(0, 0, 0));
				enemy->AddSphereCollider(2.0f, enemy->GetPosition());
				enemy->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
				enemy->SetHealth(100);
				physicsObjects.push_back(enemy);
			}

			if (input == 2)
			{
				NormalEnemy *enemy = new NormalEnemy();
				*enemy = *masterAsteroid;
				enemy->SetValues(glm::vec3(i * 5, (rand() % 5 - 5), j * -10), glm::vec3(0, 0, 0));
				enemy->AddSphereCollider(2.0f, enemy->GetPosition());
				enemy->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
				enemy->SetHealth(100);
				physicsObjects.push_back(enemy);
			}

			temp.push_back(input);
			tempString = NULL;
		}
		coordinates.push_back(temp);
		temp.clear();
	}
	int k = 0;


	/*bool checkPositionValidity = true;
	int numberOfObjects = 20;
	NormalEnemy *masterEnemy = new NormalEnemy();
	masterEnemy->InitPath("Models/SpaceCraft/Wraith Raider Starship.obj");
	for (int i = 0; i < numberOfObjects; i++)
	{
		NormalEnemy *enemy = new NormalEnemy();
		*enemy = *masterEnemy;
		while (checkPositionValidity == true)
		{
			enemy->SetValues(glm::vec3(rand() % 10, (rand() % 5 - 5), (rand() % 50) * -1),glm::vec3(0,0,0));
			auto positionTuple = make_tuple(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
			if (IsPositionValid(positionTuple,3) == true)
			{
				checkPositionValidity = false;
			}
		}
		checkPositionValidity = true;
		auto positionTuple = make_tuple(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
		mapPositions[positionTuple] = 2;
		enemy->AddSphereCollider(2.0f, enemy->GetPosition());
		enemy->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
		enemy->SetHealth(100);
		physicsObjects.push_back(enemy);
	}*/
}

double calcFPS(double theTimeInterval, string theWindowTitle)
{
	// Static values which only get initialised the first time the function runs
	static double t0Value = glfwGetTime(); // Set the initial time to now
	static int    fpsFrameCount = 0;             // Set the initial FPS frame count to 0
	static double fps = 0.0;           // Set the initial FPS value to 0.0

									   // Get the current time in seconds since the program started (non-static, so executed every time)
	double currentTime = glfwGetTime();

	// Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s)
	// Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least.
	if (theTimeInterval < 0.1)
	{
		theTimeInterval = 0.1;
	}
	if (theTimeInterval > 10.0)
	{
		theTimeInterval = 10.0;
	}

	// Calculate and display the FPS every specified time interval
	if ((currentTime - t0Value) > theTimeInterval)
	{
		// Calculate the FPS as the number of frames divided by the interval in seconds
		fps = (double)fpsFrameCount / (currentTime - t0Value);

		// If the user specified a window title to append the FPS value to...
		if (theWindowTitle != "NONE")
		{
			// Convert the fps value into a string using an output stringstream
			std::ostringstream stream;
			stream << fps;
			std::string fpsString = stream.str();

			// Append the FPS value to the window title details
			theWindowTitle += " | FPS: " + fpsString;

			// Convert the new window title to a c_str and set it
			const char* pszConstString = theWindowTitle.c_str();
			glfwSetWindowTitle(window, pszConstString);
		}
		else // If the user didn't specify a window to append the FPS to then output the FPS to the console
		{
			std::cout << "FPS: " << fps << std::endl;
		}

		// Reset the FPS frame counter and set the initial time to be now
		fpsFrameCount = 0;
		t0Value = glfwGetTime();
	}
	else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter
	{
		fpsFrameCount++;
	}

	// Return the current FPS - doesn't have to be used if you don't want it!
	return fps;
}


