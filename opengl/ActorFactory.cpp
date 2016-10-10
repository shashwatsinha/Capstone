#include "ActorFactory.h"



ActorFactory::ActorFactory()
{
}

void ActorFactory::SetMass(float mass)
{
	ourModel->SetMass(mass);
}



void ActorFactory::InitPath(GLchar * path)
{
	ourModel->InitPath(path);
}


void ActorFactory::InitActor(GLchar * path, int typeActor, int typeModel)
{
	typeOfActor = typeActor;
	typeOfModel = typeModel;

	if (typeActor == 0 && typeModel==0)
	{

		ourModel = new EnvironmentObject();
		ourModel->InitPath(path);
		ourModel->InitializeRigidBody();
	}

	if (typeActor == 0 && typeModel == 1)
	{
		ourModel = new NormalEnemy();
		ourModel->InitPath(path);
		physicsObjects.push_back((NormalEnemy*)ourModel);
	}

	if (typeActor == 0 && typeModel == 3)
	{
		/*ourModel = new Bullet();
		ourModel->InitPath(path);
		physicsObjects.push_back((Bullet*)ourModel);*/
	}
}

btRigidBody* ActorFactory::GetRigidBody(Model *ourModel)
{
		return ourModel->body;
}

btRigidBody* ActorFactory::GetRigidBody()
{
	return ourModel->body;
}

btCollisionObject* ActorFactory::GetCollisionObject(Model *ourModel)
{
	return ourModel->mPlayerObject;
}

btCollisionObject* ActorFactory::GetCollisionObject()
{
	return ourModel->mPlayerObject;
}

void ActorFactory::SetGameObjID(int id)
{
	ourModel->GameObjID = id;
}

int ActorFactory::GetGameObjID()
{
	return ourModel->GameObjID;
}

btTransform  ActorFactory::GetstartTransform()
{
	return ourModel->startTransform;
}

void ActorFactory::UpdateActor(Shader *shader)
{
	//ourModel->Draw(shader);
	for (int i = 0; i < physicsObjects.size(); i++)
	{
		physicsObjects[i]->Draw(shader);
	}

	for (int i = 0; i < nonPhysicsObjects.size(); i++)
	{
		nonPhysicsObjects[i]->Draw(shader);
	}
}

void ActorFactory::Draw(Shader *shader)
{
	ourModel->Draw(shader);
}

void ActorFactory::UpdateActor(Shader *shader,bool test)
{
		ourModel->Draw(shader);
}

void ActorFactory::SetPosition(Model *ourModel,glm::vec3 pos)
{
	if (typeOfActor == 0)
	{
		ourModel->SetPosition(pos);
	}
}

void ActorFactory::SetPosition( glm::vec3 pos)
{
	if (typeOfActor == 0)
	{
		ourModel->SetPosition(pos);
	}
}


glm::vec3 ActorFactory::GetPosition()
{
	if (typeOfActor == 0)
	{
		//cout << ourModel->GetPosition().x << " " << ourModel->GetPosition().y << " " << ourModel->GetPosition().z << endl;
		return ourModel->GetPosition();
	}

	return glm::vec3(0, 0, 0);
}

void ActorFactory::UpdatePhysicsPropertiesForObject(Model *ourModel)
{
	if (typeOfActor == 0)
	{
		ourModel->SetDefaultMass();
		ourModel->InitializeRigidBody();
		ourModel->body->activate(true);

	}
}

void ActorFactory::UpdatePhysicsPropertiesForObject()
{
	if (typeOfActor == 0)
	{
		ourModel->SetDefaultMass();
		ourModel->InitializeRigidBody();
		ourModel->body->activate(true);

	}
}

void ActorFactory::SetScale(Model *ourModel, glm::vec3 sc)
{
	if (typeOfActor == 0)
	{
		ourModel->SetScale(sc);
	}
}

void ActorFactory::SetScale( glm::vec3 sc)
{
	if (typeOfActor == 0)
	{
		ourModel->SetScale(sc);
	}
}

glm::vec3 ActorFactory::GetScale(glm::vec3 sc)
{
	if (typeOfActor == 0)
	{
		return ourModel->GetScale();
	}

	return glm::vec3(1, 1, 1);
}

void ActorFactory::AddAI()
{
	typeOfModel = 1;
}

void ActorFactory::ProcessAI(glm::vec3 playerPos)
{
	if (this->typeOfModel == 1)
	{
		dynamic_cast<NormalEnemy*>(ourModel)->ProcessAI(playerPos);
	}
}

ActorFactory::~ActorFactory()
{}

std::vector<Model*> ActorFactory::GetPhysicsObjectsList()
{
	return physicsObjects;
}

