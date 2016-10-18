#define GLEW_STATIC
#include "windows.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "Camera.h"
#include "ActorFactory.h"
#include "btBulletDynamicsCommon.h"
#include "Skybox.h"

#include <queue>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include "Physics.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "assimp-vc130-mt.lib")
#pragma comment(lib,"BulletCollision_Debug.lib")
#pragma comment(lib,"BulletDynamics_Debug.lib")
#pragma comment(lib,"LinearMath_Debug.lib")
#include <iostream>

//typedef list<allocator<ActorFactory> > ActorFactoryQ;

GLuint screenWidth = 800, screenHeight = 600;
int sizeA = 1;
// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();
void InitGLFW();
void LoadDynamicsWorld();
void UpdatePhysicsWorld(float elapsedTime);
//void UpdatePhysicsWorld(float elapsedTime, list<ActorFactory*> objectList1, list<ActorFactory*> objectList2);
void AddPhysicsForModels(ActorFactory* g);
void DestroyExcessBullets();
void Shoot();
void HandleShotObjects();
void SetMassForModels(ActorFactory* g, float mass);
void GetRigidBodyState(ActorFactory* g);
void SetViewAndProjection(Shader shader, glm::mat4 view);

float x = 0;
// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLFWwindow* window;

GLfloat lastBulletFired;
GLfloat currentBulletFired;
std::vector<NormalEnemy*> enemies;
vector<Bullets*> bullets;
vector<EnvironmentObject*> envObjs;

vector<ActorFactory*> physicsObjects;

int objIdTracker = 0;
//ActorFactory *enemy = new ActorFactory();
//ActorFactory *bullets = new ActorFactory();

// The MAIN function, from here we start our application and run our Game loop
int main()
{
	LoadDynamicsWorld();

	// Init GLFW
	InitGLFW();
	// Setup and compile our shaders
	Shader shader("Shaders/nanosuit.vs", "Shaders/nanosuit.frag");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.frag");

	// Setup Skybox
	Skybox skybox;
	skybox.setupMesh();

	//create enemies
	for (int i = 0;i < 1;i++)
	{
		NormalEnemy *enemy = new NormalEnemy();
		enemy->InitPath("Models/Cube/Cube.obj");
		enemy->SetPosition(glm::vec3(i * 2, 0, 15));
		enemy->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		enemies.push_back(enemy);
		//AddPhysicsForModels(enemy);
		//SetMassForModels(enemy, 1.0);
		//enemy->SetGameObjID(objIdTracker);
		objIdTracker++;
		//enemy->AddAI();
		enemy->mass = 0.0f;
		enemy->mPlayerBox = new btBoxShape(btVector3(0.1, 0.1, 0.1));
		enemy->ActivatePhysics();
		Physics::instance()->dynamicsWorld->addRigidBody(enemy->body);
		Physics::instance()->dynamicsWorld->addCollisionObject(enemy->mPlayerObject);


	}

	for (int i = 0;i < 0;i++)
	{
		EnvironmentObject *env1 = new EnvironmentObject();
		env1->InitPath("Models/Cube/Cube.obj");
		env1->SetPosition(glm::vec3(i*2, 5, 10));
		env1->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
		envObjs.push_back(env1);
		//AddPhysicsForModels(env1);
		//SetMassForModels(env1, 1.0);
		env1->mass = 0.0f;
		env1->mPlayerBox = new btBoxShape(btVector3(0.1, 0.1, 0.1));
		env1->ActivatePhysics();
		Physics::instance()->dynamicsWorld->addRigidBody(env1->body);
		Physics::instance()->dynamicsWorld->addCollisionObject(env1->mPlayerObject);
	}


	//enemy->InitActor("Models/Cube/Cube.obj", 0, 1);
	//enemy->SetPosition(enemy->GetPhysicsObjectsList()[enemy->GetPhysicsObjectsList().size() - 1],glm::vec3(10, 10, 10));
	//enemy->SetScale(enemy->GetPhysicsObjectsList()[enemy->GetPhysicsObjectsList().size() - 1],glm::vec3(0.2f, 0.2f, 0.2f));
	//enemy->SetGameObjID(objIdTracker);
	//objIdTracker++;
	//enemy->AddAI();

	

	// add every object with physics property soon after creation.

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glm::mat4 model[1];
	// Game loop
	//AddPhysicsForModels(enemy);
	//SetMassForModels(enemy,1.0);
	
	// Draw in wireframe

	float bulletdestroyTimer = 3.0;
	int x = 0;
	while (!glfwWindowShouldClose(window))
	{
		//GetRigidBodyState(enemy);

		// destroy shot objects after some time
		if (bulletdestroyTimer < 0)
		{
			bulletdestroyTimer = 3.0;
			DestroyExcessBullets();
		}
		//HandleShotObjects();

		// Set frame time
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		bulletdestroyTimer = bulletdestroyTimer - deltaTime;

		UpdatePhysicsWorld(currentFrame);
		//UpdatePhysicsWorld(currentFrame,physicsGameObjects,bullets);
		// Check and call events
		glfwPollEvents();
		Do_Movement();

		// Clear the colorbuffer
		glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// SkyBox
		// Draw skybox first
		glDepthMask(GL_FALSE);// Remember to turn depth writing off
		skyboxShader.Use();
		glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix()));	// Remove any translation component of the view matrix
		SetViewAndProjection(skyboxShader, view);
		skybox.Draw(&skyboxShader);
		glDepthMask(GL_TRUE);
		// SkyBox

		shader.Use();   // <-- Don't forget this one!
		view = camera.GetViewMatrix();
		SetViewAndProjection(shader, view);
		// Draw the loaded model
		//enemy->GetPosition();
		//enemy->UpdateActor(&shader);
		//enemy->ProcessAI(camera.Position);

		for (int i = 0;i < envObjs.size();i++)
		{
			envObjs[i]->Draw(&shader);
		}

		for (int i = 0;i < enemies.size();i++)
		{
			//enemies[i]->TestMove();
			enemies[i]->ProcessAI(camera.GetPosition());
			enemies[i]->Draw(&shader); 
			
			
		}

		for (int i = 0;i < bullets.size();i++)
		{
			bullets[i]->Draw(&shader);
		}

		//bullets->UpdateActor(&shader);

		//if (bullets.size() > 1)
		//{
		//	int counter = 0;
		//	float  x=0;
		//	float  y=0;
		//	float  z=0;
		//	for each (ActorFactory *obj in bullets)
		//	{
		//		counter++;
		//		if(counter>1){
		//			break;
		//		}
		//		x = obj->GetPosition().x;
		//		y = obj->GetPosition().y;
		//		z = obj->GetPosition().z;
		//	}

		////	cout << bullets.size() << " " << x << " " << y << " " << z << endl;
		//}
		
		// Swap the buffers

		if (bullets.size())
		{
			cout << bullets.at(0)->GetMass() << endl;
		}

		glfwSwapBuffers(window);

	}

	// Clean up behind ourselves like good little programmers
	glfwTerminate();
	return 0;
}

void SetViewAndProjection(Shader shader, glm::mat4 view) {
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void Shoot()
{
	//lastBulletFired = glfwGetTime();
	//bullets->InitActor("Models/Cube/Cube.obj", 0, 1);
	//bullets->SetPosition(bullets->GetPhysicsObjectsList()[bullets->GetPhysicsObjectsList().size() - 1], glm::vec3(camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z));


	//bullets->SetScale(bullets->GetPhysicsObjectsList()[bullets->GetPhysicsObjectsList().size() - 1], glm::vec3(0.2f, 0.2f, 0.2f));
	//bullets->SetGameObjID(objIdTracker);
	//objIdTracker++;
	//cout << "Shooting";
	//AddPhysicsForModels(bullets);
		//bullets.push_back(ac1);
		//bulletsBodies.push_back(ac1->GetCollisionObject());

	Bullets *bullet = new Bullets();
	bullet->InitPath("Models/Cube/Cube.obj");
	bullet->SetPosition(camera.Position);
	bullet->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	bullets.push_back(bullet);

	bullet->mass = 1.0f;
	bullet->mPlayerBox = new btBoxShape(btVector3(0.1, 0.1, 0.1));

	bullet->ActivatePhysics();
	Physics::instance()->dynamicsWorld->addRigidBody(bullet->body);
	Physics::instance()->dynamicsWorld->addCollisionObject(bullet->mPlayerObject);


	
	float impulse = 10.0;
	bullet->body->applyCentralImpulse(btVector3(camera.Front.x, camera.Front.y, camera.Front.z)*impulse);
	
}

void HandleShotObjects()
{

	/*for each (ActorFactory *obj in bullets)
	{
		obj->GetRigidBody()->applyCentralImpulse(btVector3(0.0f, 0.0f, 1.0f));
	}*/
}

void DestroyExcessBullets()
{
	//int removeObjectsCounter;
	//removeObjectsCounter = bullets.size() / 2;

	//if (removeObjectsCounter<20)
	//{
	//	removeObjectsCounter = removeObjectsCounter / 2;
	//}
	//for (int i = 0;i < removeObjectsCounter;i++)
	//{
	//	ActorFactory *temp;
	//	temp = bullets.front();
 //     		bullets.pop_front();
	//	physicsGameObjects.erase(std::remove(physicsGameObjects.begin(), physicsGameObjects.end(), temp), physicsGameObjects.end());
	//	delete temp;

	//}
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

	float shootinterval = 1.0;

	GLfloat currentFrame1 = 0.0f;
	GLfloat lastFrame1 = 0.0f;
	if (keys[GLFW_KEY_SPACE])
	{
		// Set frame time
		currentBulletFired = glfwGetTime();
		shootinterval = currentBulletFired - lastBulletFired;
		
			// destroy shot objects after some time
 			if (shootinterval > 0.5f)
			{
				Shoot();
				shootinterval = 10.0;
			}
			//HandleShotObjects();


	}

	lastBulletFired = currentBulletFired;
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

void LoadDynamicsWorld()
{
	//physics = new Physics();
}

void AddPhysicsForModels( Model* g)
{
	g->ActivatePhysics();
	Physics::instance()->dynamicsWorld->addRigidBody(g->body);
	Physics::instance()->dynamicsWorld->addCollisionObject(g->mPlayerObject);
}

void AddPhysicsForModels(EnvironmentObject* g)
{
	g->ActivatePhysics();
	Physics::instance()->dynamicsWorld->addRigidBody(g->body);
	Physics::instance()->dynamicsWorld->addCollisionObject(g->mPlayerObject);
	//physicsObjects.push_back(g);
}


//void SetMassForModels(ActorFactory* g,float mass)
//{
//	for (int i = 0; i < g->GetPhysicsObjectsList().size(); i++)
//	{
//
//		g->SetMass(mass);
//		g->GetRigidBody(g->GetPhysicsObjectsList()[i])->setGravity(btVector3(0, 0, 0));
//		//g->GetRigidBody(g->GetPhysicsObjectsList()[i])->setActivationState(0);
//
//	}
//
//}
//
//void GetRigidBodyState(ActorFactory* g)
//{
//	for (int i = 0; i < g->GetPhysicsObjectsList().size(); i++)
//	{
//		int activestate = g->GetRigidBody(g->GetPhysicsObjectsList()[i])->getActivationState();
//		//cout << activestate << endl;
//	}
//}

void UpdatePhysicsWorld(float elapsedTime)
{
	// fixed 1/60 timestep
	Physics::instance()->dynamicsWorld->stepSimulation(1 / 10.0f, 1);

	glm::vec3 mat;
	const btCollisionObjectArray& objectArray = Physics::instance()->dynamicsWorld->getCollisionObjectArray();
	int count = 0;




	for (int i = 0; i < envObjs.size(); i++)
	{
		envObjs.at(i)->body->activate(true);

		btRigidBody* pBody = envObjs.at(i)->body;
		if (pBody && pBody->getMotionState())
		{
			btTransform trans = envObjs.at(i)->startTransform;
			pBody->getMotionState()->getWorldTransform(trans);

			glm::vec3 pos;
			pos.x = trans.getOrigin().getX();
			pos.y = trans.getOrigin().getY();
			pos.z = trans.getOrigin().getZ();
			envObjs.at(i)->SetPosition(pos);
			//physicsGameObjects.at(i)->SetRotation(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
		}
	}

	for (int i = 0; i < enemies.size(); i++)
	{
		enemies.at(i)->body->activate(true);

		btRigidBody* pBody = enemies.at(i)->body;
		if (pBody && pBody->getMotionState())
		{
			btTransform trans = enemies.at(i)->startTransform;
			pBody->getMotionState()->getWorldTransform(trans);

			glm::vec3 pos;
			pos.x = trans.getOrigin().getX();
			pos.y = trans.getOrigin().getY();
			pos.z = trans.getOrigin().getZ();
			//What is this
			//enemies.at(i)->SetPosition(pos);
			//physicsGameObjects.at(i)->SetRotation(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
		}
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		bullets.at(i)->body->activate(true);

		btRigidBody* pBody = bullets.at(i)->body;
		if (pBody && pBody->getMotionState())
		{
			btTransform trans = bullets.at(i)->startTransform;
			pBody->getMotionState()->getWorldTransform(trans);

			glm::vec3 pos;
			pos.x = trans.getOrigin().getX();
			pos.y = trans.getOrigin().getY();
			pos.z = trans.getOrigin().getZ();
			bullets.at(i)->SetPosition(pos);
			//physicsGameObjects.at(i)->SetRotation(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
		}
	}

	//collision detection on all objects
	//DetectCollisions();

	//collision detection for all objects
	//Physics::instance()->DetectCollision(enemies,bullets);

	for (int i=0;i<enemies.size();i++)
	{
		for (int j = 0; j < bullets.size(); j++)
		{
			if (Physics::instance()->DetectCollision(enemies[i]->mPlayerObject, bullets[j]->mPlayerObject, Physics::instance()))
			{
				cout << "collision happened";
			}
		}
	}
}
