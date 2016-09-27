#pragma once
#include "Model.h"
#include "NormalEnemy.h"
#include "EnvironmentObject.h"
#include "Bullet.h"
class ActorFactory
{
public:
	ActorFactory();
	//Inititalize the actor, if typeActor=0, then model, 1 = sound. typeModel= 0 denotes a non-movable Object, 1 denotes an enemy, 2 denotes a random moving object,3 denotes a bullet
	void InitActor(GLchar* path, int typeActor,int typeModel);

	btRigidBody* GetRigidBody();

	btCollisionObject* GetCollisionObject();

	void SetGameObjID(int id);

	int GetGameObjID();

	btTransform GetstartTransform();

	//Update the Actor per frame if necesssary
	void UpdateActor(Shader *shader);

	//Set position of Actor
	void SetPosition(glm::vec3 pos);

	void SetPosition(XMFLOAT3 pos);

	//Get Position of Actor
	glm::vec3 GetPosition();

	void UpdatePhysicsPropertiesForObject();

	//Set scale of Actor
	void SetScale(glm::vec3 sc);

	//Get scale of Actor
	glm::vec3 GetScale(glm::vec3 sc);

	//Add AI to an Actor
	void AddAI();

	//Process AI of an Actor
	void ProcessAI(glm::vec3);
	~ActorFactory();

private:
	unsigned int actorID;
	int typeOfActor;
	int typeOfModel;
	glm::vec3 position;
	glm::vec3 scale;
	Model *ourModel;
	vector<Model*> physicsObjects;
	vector<Model*> nonPhysicsObjects;
};

