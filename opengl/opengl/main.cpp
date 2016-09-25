#define GLEW_STATIC
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "Camera.h"
#include "ActorFactory.h"
#include "btBulletDynamicsCommon.h"
#include <list>
using std::list;
#include <queue>

#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>



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

float x = 0;
// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLFWwindow* window;

Physics* physics;
std::list<ActorFactory*> physicsGameObjects;
//list<ActorFactory*> bullets;
std::list<ActorFactory*> bullets;
int objIdTracker = 0;
ActorFactory *enemy = new ActorFactory();


struct rCallBack : public btCollisionWorld::ContactResultCallback
{
	virtual   btScalar   addSingleResult(btManifoldPoint& cp, const btCollisionObject* colObj0, int partId0, int index0, const btCollisionObject* colObj1, int partId1, int index1)
	{
		btVector3 ptA = cp.getPositionWorldOnA();
		btVector3 ptB = cp.getPositionWorldOnB();
		std::cout << "HIT" << std::endl;
		return 0;
	}
};


// The MAIN function, from here we start our application and run our Game loop
int main()
{
	LoadDynamicsWorld();



	// Init GLFW
	InitGLFW();
	// Setup and compile our shaders
	Shader shader("Shaders/nanosuit.vs", "Shaders/nanosuit.frag");

	enemy->InitActor("Models/Cube/Cube.obj", 0, 1);
	enemy->SetPosition(glm::vec3(0, 1, 0));
	enemy->SetScale(glm::vec3(0.2f, 0.2f, 0.2f));
	enemy->SetGameObjID(objIdTracker);
	objIdTracker++;
	enemy->AddAI();
	// add every object with physics property soon after creation.

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glm::mat4 model[1];
	// Game loop
	AddPhysicsForModels(enemy);
	// Draw in wireframe
	
	//ac1->GetRigidBody()->setUserPointer(physicsGameObjects.end);

	//gContactAddedCallback = callbackFunc();

	float bulletdestroyTimer = 3.0;

	while (!glfwWindowShouldClose(window))
	{
		if (bulletdestroyTimer < 0)
		{
			bulletdestroyTimer = 3.0;
			DestroyExcessBullets();
		}
		HandleShotObjects();

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

		shader.Use();   // <-- Don't forget this one!
						// Transformation matrices
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(shader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		// Draw the loaded model
		enemy->GetPosition();
		enemy->UpdateActor(&shader);
		enemy->ProcessAI(camera.Position);

		if (bullets.size() > 1)
		{
			int counter = 0;
			float  x=0;
			float  y=0;
			float  z=0;
			for each (ActorFactory *obj in bullets)
			{
				counter++;
				if(counter>1){
					break;
				}
				x = obj->GetPosition().x;
				y = obj->GetPosition().y;
				z = obj->GetPosition().z;
			}

			cout << bullets.size() << " " << x << " " << y << " " << z << endl;
		}

		//dynamicsWorld->stepSimulation(1 / 60.f, 10);

		//btTransform trans;
		//fallRigidBody->getMotionState()->getWorldTransform(trans);
		//std::cout << "sphere height: " <<trans.getOrigin().getX()<<" "<< trans.getOrigin().getY() << std::endl;
		//ac1->SetPosition(glm::vec3(trans.getOrigin().getY(), 0, 0));
		
		// Swap the buffers
		glfwSwapBuffers(window);

	}

	// Clean up behind ourselves like good little programmers
	glfwTerminate();
	return 0;
}

void Shoot()
{
	ActorFactory *ac1 = new ActorFactory();
	ac1->InitActor("Models/Cube/Cube.obj", 0, 1);
	ac1->SetPosition(glm::vec3(camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z));
	ac1->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	ac1->SetGameObjID(objIdTracker);
	objIdTracker++;

	AddPhysicsForModels(ac1);
	bullets.push_back(ac1);

}

void HandleShotObjects()
{

	for each (ActorFactory *obj in bullets)
	{
		obj->GetRigidBody()->applyCentralImpulse(btVector3(0.0f, 0.0f, 1.0f));
	}
}

void DestroyExcessBullets()
{
	int removeObjectsCounter;
	removeObjectsCounter = bullets.size() / 2;

	if (removeObjectsCounter<20)
	{
		removeObjectsCounter = removeObjectsCounter / 2;
	}
	for (int i = 0;i < removeObjectsCounter;i++)
	{
		ActorFactory *temp;
		temp = bullets.front();
      		bullets.pop_front();
		physicsGameObjects.remove(temp);
		delete temp;

	}
}

//bool callbackFunc(btManifoldPoint& cp, const btCollisionObject* obj1, int id1, int index1, const btCollisionObject* obj2, int id2, int index2)
//{
//	((bulletObject*)obj1->getUserPointer())->hit = true;
//
//	((bulletObject*)obj2->getUserPointer())->hit = true;
//	return false;
//}


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
	//if (keys[GLFW_KEY_K])
	//{
	//	physicsGameObjects.at(0)->GetRigidBody()->applyCentralImpulse(btVector3(1.0f, 0.0f, 0.0f));
	//}
	//if (keys[GLFW_KEY_J])
	//{
	//	physicsGameObjects.at(0)->GetRigidBody()->applyCentralImpulse(btVector3(-1.0f, 0.0f, 0.0f));
	//}
	//if (keys[GLFW_KEY_I])
	//{
	//	physicsGameObjects.at(0)->GetRigidBody()->applyCentralImpulse(btVector3(0.0f, 0.0f, 1.0f));
	//}
	//if (keys[GLFW_KEY_M])
	//{
	//	physicsGameObjects.at(0)->GetRigidBody()->applyCentralImpulse(btVector3(0.0f, 0.0f, -1.0f));
	//}

	if (keys[GLFW_KEY_SPACE])
	{
		Shoot();
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

void LoadDynamicsWorld()
{
	physics = new Physics();
}

void AddPhysicsForModels( ActorFactory* g)
{

	g->UpdatePhysicsPropertiesForObject();

	physicsGameObjects.push_back(g);
	physics->dynamicsWorld->addRigidBody(g->GetRigidBody());
	physics->dynamicsWorld->addCollisionObject(g->GetCollisionObject());

}

void DetectCollision()
{
	//physics->dynamicsWorld->performDiscreteCollisionDetection();
	////Also tried doing this with stepSimulation(deltaTime, 7), but nothing changed.
	////stepSimulation seems to only be for letting Bullet set world Transforms?

	////check collisions with player
	//resultCallback = rCallBack;
	//physics->dynamicsWorld->contactTest(enemy->GetCollisionObject(), resultCallback);
	//int numManifolds = physics->dynamicsWorld->getDispatcher()->getNumManifolds();
	//if (numManifolds > 0)
	//{
	//	//there's a collision, execute blah blah blah
	//}
}

void DetectCollisions()
{
	physics->dynamicsWorld->performDiscreteCollisionDetection();

	int numManifolds = physics->dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0;i<numManifolds;i++)

	{

		btPersistentManifold* contactManifold = physics->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

		int numContacts = contactManifold->getNumContacts();

		for (int j = 0;j<numContacts;j++)

		{

			btManifoldPoint& pt = contactManifold->getContactPoint(j);

			if (pt.getDistance()<0.f)

			{

				const btVector3& ptA = pt.getPositionWorldOnA();

				const btVector3& ptB = pt.getPositionWorldOnB();

				const btVector3& normalOnB = pt.m_normalWorldOnB;

			}

		}

	}
}

void UpdatePhysicsWorld(float elapsedTime)
{
	//list<ActorFactory*> objectList;
	//objectList.insert(objectList.end(), objectList1.begin(), objectList1.end());
	//objectList.insert(objectList.end(), objectList2.begin(), objectList2.end());

	// fixed 1/60 timestep
	physics->dynamicsWorld->stepSimulation(1 / 10.0f, 1);

	XMFLOAT3 mat;
	const btCollisionObjectArray& objectArray = physics->dynamicsWorld->getCollisionObjectArray();
	int count = 0;
	for each (ActorFactory *obj in physicsGameObjects)
	{

		obj->GetRigidBody()->activate(true);

		btRigidBody* pBody = obj->GetRigidBody();
		if (pBody && pBody->getMotionState())
		{
			btTransform trans = obj->GetstartTransform();
			pBody->getMotionState()->getWorldTransform(trans);
			XMFLOAT3 pos = XMFLOAT3(trans.getOrigin());
			obj->SetPosition(pos);
			//obj->SetRotation(trans.getRotation().getX(), trans.getRotation().getY(), trans.getRotation().getZ());
			//obj->SetWorldMatrix();
		}
	}

	DetectCollisions();

}
