#pragma once
class Model;
#include "NormalEnemy.h"
#include "EnvironmentObject.h"
#include "Bullets.h"
class ActorFactory
{
public:
	ActorFactory();
	void SetMass(float mass);
	void InitPath(GLchar * path);
	//Inititalize the actor, if typeActor=0, then model, 1 = sound. typeModel= 0 denotes a non-movable Object, 1 denotes an enemy, 2 denotes a random moving object,3 denotes a bullet
	void InitActor(GLchar* path, int typeActor, int typeModel);

	btRigidBody* GetRigidBody(Model *ourModel);

	btRigidBody * GetRigidBody();

	btCollisionObject* GetCollisionObject(Model *ourModel);

	btCollisionObject * GetCollisionObject();

	void SetGameObjID(int id);

	int GetGameObjID();

	btTransform GetstartTransform();

	//Update the Actor per frame if necesssary
	void UpdateActor(Shader *shader);

	void Draw(Shader * shader);

	void UpdateActor(Shader * shader, bool test);

	//Set position of Actor
	void SetPosition(Model *ourModel,glm::vec3 pos);

	void SetPosition(glm::vec3 pos);

	//Get Position of Actor
	glm::vec3 GetPosition();

	void UpdatePhysicsPropertiesForObject(Model *ourModel);

	void UpdatePhysicsPropertiesForObject();

	//Set scale of Actor
	void SetScale(Model *ourModel, glm::vec3 sc);

	void SetScale(glm::vec3 sc);

	//Get scale of Actor
	glm::vec3 GetScale(glm::vec3 sc);

	//Add AI to an Actor
	void AddAI();

	//Process AI of an Actor
	void ProcessAI(glm::vec3);
	~ActorFactory();

	std::vector<Model*> GetPhysicsObjectsList();
private:
	unsigned int actorID;
	int typeOfActor;
	int typeOfModel;
	glm::vec3 position;
	glm::vec3 scale;
	Model *ourModel;

	std::vector<Model*> nonPhysicsObjects;
	std::vector<Model*> physicsObjects;
};

