#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "Camera.h"
#include "ActorFactory.h"
#include <stdlib.h>    
#include <time.h>
#include <map>
#include <tuple>
#include "Bullet.h"
#include "Skybox.h"

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
bool IsPositionValid(std::tuple<int, int, int>);
void DetectCollisions();
void Shoot();
void SetViewAndProjection(Shader shader, glm::mat4 view);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLFWwindow* window;
vector<NormalEnemy*> physicsObjects;
vector<Bullet*> bullets;
// The MAIN function, from here we start our application and run our Game loop
map<tuple<int,int,int>, int> mapPositions;

float currentBulletFired = 0;
float previousBulletFired = 0;
int main()
{
	// Init GLFW
	InitGLFW();
	// Setup and compile our shaders
	Shader shader("Shaders/nanosuit.vs", "Shaders/nanosuit.frag");
	Shader skyboxShader("Shaders/vertexShader_Skybox.vs", "Shaders/fragmentShader_Skybox.frag");

	srand(time(NULL));

	Skybox skybox;
	skybox.setupMesh();

	GenerateEnvironment();
	//ac1->AddAI();
	
	// Draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glm::mat4 model[1];
	// Game loop



	while (!glfwWindowShouldClose(window))
	{
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

		// Draw skybox first
		glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skyboxShader.Use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		SetViewAndProjection(skyboxShader, view);
		skybox.Draw(&skyboxShader);
		glDepthMask(GL_TRUE);
		
		shader.Use();   // <-- Don't forget this one!
						// Transformation matrices
		view = camera.GetViewMatrix();
		SetViewAndProjection(shader, view);
		// Draw the loaded model
		
		for (int i = 0; i < physicsObjects.size(); i++)
		{
			physicsObjects[i]->ProcessAI(camera.Position);
			physicsObjects[i]->UpdateCollider();
			physicsObjects[i]->Draw(&shader);
			for (int j = 0; j < physicsObjects[i]->enemyBullets.size(); j++)
			{
				physicsObjects[i]->enemyBullets[j]->UpdateCollider();
				physicsObjects[i]->enemyBullets[j]->Draw(&shader);
			}
		}

		for (int i = 0; i < bullets.size(); i++)
		{
			bullets[i]->UpdateCollider();
			bullets[i]->Draw(&shader);
		}

		
		DetectCollisions();
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	
	glfwTerminate();
	return 0;
}

void SetViewAndProjection(Shader shader, glm::mat4 view) {
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

#pragma region "User input"

// Moves/alters the camera positions based on user input
void Do_Movement()
{
	// Camera controls
	if (keys[GLFW_KEY_W])
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (keys[GLFW_KEY_P])
	{
		cout<<camera.GetPosition().x<< " " << camera.GetPosition().y<<" "<< camera.GetPosition().z<<endl;
	}
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

	camera.ProcessMouseMovement(xoffset, yoffset,true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
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

}

bool IsPositionValid(std::tuple<int,int,int> positionTuple)
{
	bool flag = false;
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

			if ((d1 + d2 + d3) < 25 && (d1 + d2 + d3)>0)
			{
				mapPositions.erase(positionTuple);
				flag = false;
				return flag;
			}
		}
		return flag;
	}
	mapPositions.erase(positionTuple);
	return flag;
}

void DetectCollisions()
{
	for (int i = 0; i < physicsObjects.size(); i++)
	{
		for (int j = 0; j < bullets.size(); j++)
		{
			glm::vec3 distance = physicsObjects[i]->GetPosition() - bullets[j]->GetPosition();
			float sumOfRadii = physicsObjects[i]->GetCollider()->GetRadius() + bullets[j]->GetCollider()->GetRadius();
			float length = (distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z);
			if (length < (sumOfRadii * sumOfRadii))
			{
				Bullet *bullet = bullets[j];
				bullets.erase(bullets.begin() + j);
				cout << "Collision Happened";
				delete bullet;

				physicsObjects[i]->ReduceHealth(10);
				if (physicsObjects[i]->GetHealth() <= 0)
				{
					NormalEnemy *enemy = physicsObjects[i];
					physicsObjects.erase(physicsObjects.begin() + i);
					delete enemy;
				}
			}
		}
	}
}

void Shoot()
{
	Bullet *enemy = new Bullet();
	enemy->InitPath("Models/Bullet/Bullet.obj");
	glm::vec3 shootPosition = glm::vec3(camera.GetPosition().x, camera.GetPosition().y - 1.0f, camera.GetPosition().z);
	enemy->SetPosition(shootPosition);
	enemy->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	enemy->AddSphereCollider(2.0f, enemy->GetPosition());
	glm::vec3 bulletDirection = glm::normalize(camera.Front);
	float bulletSpeed = 4.0f;
	bulletDirection = glm::vec3(bulletDirection.x * bulletSpeed,
								bulletDirection.y * bulletSpeed,
								bulletDirection.z * bulletSpeed);
	enemy->SetVelocity(bulletDirection);
	bullets.push_back(enemy);
}

void GenerateEnvironment()
{
	/*bool checkPositionValidity = true;
	int numberOfObjects = 20;
	for (int i = 0; i < numberOfObjects; i++)
	{
		NormalEnemy *enemy = new NormalEnemy();
		enemy->InitPath("Models/SpaceShip/SpaceShip.obj");
		while (checkPositionValidity == true)
		{
			enemy->SetPosition(glm::vec3(rand() % 10, (rand() % 5 - 5), (rand() % 50) * -1));
			auto positionTuple = make_tuple(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
			if (IsPositionValid(positionTuple) == true)
			{
				checkPositionValidity = false;
			}
		}
		checkPositionValidity = true;
		auto positionTuple = make_tuple(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
		mapPositions[positionTuple] = 2;
		enemy->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		enemy->AddSphereCollider(2.0f, enemy->GetPosition());
		enemies.push_back(enemy);
	}*/
	
	/*enemy->InitActor("Models/Bullet/Bullet.obj", 0, 0);
	ac1->SetPosition(glm::vec3(0, 0, -5));
	ac1->SetScale(glm::vec3(1, 1, 1));*/

	NormalEnemy *enemy1 = new NormalEnemy();
	enemy1->InitPath("Models/SpaceShip/SpaceShip.obj");
	enemy1->SetPosition(glm::vec3(5,0,-20));
	enemy1->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	enemy1->AddSphereCollider(2.0f, enemy1->GetPosition());
	enemy1->SetVelocity(glm::vec3(0, 0, 0));
	enemy1->SetHealth(100);
	physicsObjects.push_back(enemy1);

	Bullet *enemy = new Bullet();
	enemy->InitPath("Models/Bullet/Bullet.obj");
	enemy->SetPosition(glm::vec3(-5, 0, 0));
	enemy->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	enemy->AddSphereCollider(2.0f, enemy->GetPosition());
	enemy->SetVelocity(glm::vec3(0.001, 0, 0));
	bullets.push_back(enemy);
}
