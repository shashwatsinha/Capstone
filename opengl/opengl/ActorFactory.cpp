#include "ActorFactory.h"



ActorFactory::ActorFactory()
{
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
	}
}

btRigidBody* ActorFactory::GetRigidBody()
{
		return ourModel->body;
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
	ourModel->Draw(shader);
}

void ActorFactory::SetPosition(glm::vec3 pos)
{
	if (typeOfActor == 0)
	{
		ourModel->SetPosition(pos);
	}
}

void ActorFactory::SetPosition(XMFLOAT3 pos)
{
	if (typeOfActor == 0)
	{
		glm::vec3 position;
		position.x = pos.x;
		position.y = pos.y;
		position.z = pos.z;
		ourModel->SetPosition(position);
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

void ActorFactory::UpdatePhysicsPropertiesForObject()
{
	if (typeOfActor == 0)
	{
		ourModel->SetDefaultMass();
		ourModel->InitializeRigidBody();
		ourModel->body->activate(true);

	}
}

void ActorFactory::SetScale(glm::vec3 sc)
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

