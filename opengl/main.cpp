#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "Camera.h"
#include "ActorFactory.h"
#include <reactphysics3d.h>
#include <stdlib.h>    
#include <time.h>
#include <map>
#include <tuple>


#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "assimp-vc130-mt.lib")
#pragma comment(lib,"BulletCollision_Debug.lib")
#pragma comment(lib,"BulletDynamics_Debug.lib")
#pragma comment(lib,"LinearMath_Debug.lib")
#pragma comment(lib,"reactphysics3d.lib")
#include <iostream>
using namespace reactphysics3d;

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

float x = 0;
// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLFWwindow* window;
ActorFactory *ac1 = new ActorFactory();

// The MAIN function, from here we start our application and run our Game loop
map<tuple<int,int,int>, int> mapPositions;
int main()
{
	// Init GLFW
	InitGLFW();
	// Setup and compile our shaders
	Shader shader("Shaders/nanosuit.vs", "Shaders/nanosuit.frag");
	srand(time(NULL));

	
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

		shader.Use();   // <-- Don't forget this one!
						// Transformation matrices
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// Draw the loaded model
		
		ac1->UpdateActor(&shader);
		//ac1->ProcessAI(camera.Position);
		
		//std::cout << "sphere height: " <<trans.getOrigin().getX()<<" "<< trans.getOrigin().getY() << std::endl;
		//ac1->SetPosition(glm::vec3(trans.getOrigin().getY(), 0, 0));
		
		// Swap the buffers
		glfwSwapBuffers(window);
	}

	// Clean up behind ourselves like good little programmers
	
	
	glfwTerminate();
	return 0;
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
	if (keys[GLFW_KEY_Q])
		x = x - 0.01f;
	if (keys[GLFW_KEY_E])
		x = x + 0.01f;
	if (keys[GLFW_KEY_P])
	{
		cout<<camera.GetPosition().x<< " " << camera.GetPosition().y<<" "<< camera.GetPosition().z<<endl;
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

void GenerateEnvironment()
{
	bool checkPositionValidity = true;
	int numberOfObjects = 20;
	for (int i = 0; i < numberOfObjects; i++)
	{
		ac1->InitActor("Models/SpaceShip/SpaceShip.obj", 0, 0);
		while (checkPositionValidity == true)
		{
			ac1->SetPosition(glm::vec3(rand() % 10, (rand() % 5 - 5), (rand() % 50) * -1));
			auto positionTuple = make_tuple(ac1->GetPosition().x, ac1->GetPosition().y, ac1->GetPosition().z);
			if (IsPositionValid(positionTuple) == true)
			{
				checkPositionValidity = false;
			}
		}
		checkPositionValidity = true;
		auto positionTuple = make_tuple(ac1->GetPosition().x, ac1->GetPosition().y, ac1->GetPosition().z);
		mapPositions[positionTuple] = 2;
		ac1->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	}

	ac1->InitActor("Models/Bullet/Bullet.obj", 0, 0);
	ac1->SetPosition(glm::vec3(0, 0, -5));
	ac1->SetScale(glm::vec3(1, 1, 1));
}