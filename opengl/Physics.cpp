#include "Physics.h"

Physics* Physics::phy_instance = 0;


Physics::Physics()
{
	CreatePhysicsWorld();
}


Physics::~Physics()
{
	delete dynamicsWorld;
	delete broadphase;
	delete collisionConfiguration;
	delete dispatcher;
	delete solver;
}

//Check if there is already an entry present in the collision tracker
bool Physics:: ExistsAlreadyInCollisionTracker(int nativeObjKey)
{
	return collisionTracker.find(nativeObjKey) != collisionTracker.end();
}

//After collision detection, add it to the tracker
void Physics::AddToCollisionTracker(int nativeid, btCollisionObject* colloidedObj)
{
	std::vector<btCollisionObject*> currentColloiders;

	if (ExistsAlreadyInCollisionTracker(nativeid))
	{
		currentColloiders = collisionTracker[nativeid];
	}
	currentColloiders.push_back(colloidedObj);
	collisionTracker[nativeid] = currentColloiders;
}

// remove the specific collision object from the tracker for a given native object
bool Physics::RemoveFromCollisionTracker(int nativeid, btCollisionObject* colloidedObj)
{
	std::vector<btCollisionObject*> currentColloiders;
	if (ExistsAlreadyInCollisionTracker(nativeid))
	{
		currentColloiders = collisionTracker[nativeid];
		currentColloiders.erase(std::remove(currentColloiders.begin(), currentColloiders.end(), colloidedObj), currentColloiders.end());
		collisionTracker[nativeid] = currentColloiders;
		return true;
	}
	else
	{
		return false;
	}
		
}

// remove the object from the tracker, usually when the game object itself is removed from the scene
bool Physics::RemoveFromCollisionTracker(int nativeid)
{
	std::unordered_map<int, std::vector<btCollisionObject*>>::iterator it;


	if (ExistsAlreadyInCollisionTracker(nativeid))
	{
		it = collisionTracker.find(nativeid);
		collisionTracker.erase(it);
		return true;
	}
	else
	{
		return false;
	}
}

// to create the main physics world
void Physics::CreatePhysicsWorld()
{
	broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	solver = new btSequentialImpulseConstraintSolver();

	// The world.
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -10, 0));

	//create a ground
	groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -2, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, -10, 0));
	groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

}

//detect collision between 2 objects
bool Physics::DetectCollision(btCollisionObject* target1, btCollisionObject* target2, Physics*physics)
{

	int numManifolds = physics->dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold = physics->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const  btCollisionObject* obA = contactManifold->getBody0();
		const  btCollisionObject* obB = contactManifold->getBody1();

		if (((target1 == obA) && (target2 == obB))|| ((target2 == obA) && (target1 == obB)))
		{
			return true;
		}

	}
	return false;
}

//detect collision, by pairs, most efficient... find collision for particular object
bool Physics::DetectCollision(btCollisionObject* target, Physics*physics)
{

	int numManifolds = physics->dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold = physics->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const  btCollisionObject* obA = contactManifold->getBody0();
		const  btCollisionObject* obB = contactManifold->getBody1();

		if ((target == obA) && (obA != obB))
		{

			return true;
		}

		if ((target == obB) && (obA != obB))
		{
			return true;
		}

	}
	return false;
}

//detect collision, by pairs, most efficient... find collision for particular object
void Physics::DetectCollision(Physics*physics)
{

	int numManifolds = physics->dynamicsWorld->getDispatcher()->getNumManifolds();
	for (int i = 0;i<numManifolds;i++)
	{
		btPersistentManifold* contactManifold = physics->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		const  btCollisionObject* obA = contactManifold->getBody0();
		const  btCollisionObject* obB = contactManifold->getBody1();

		AddToCollisionTracker(reinterpret_cast<uintptr_t>(&obB), const_cast<btCollisionObject*>(obA));
		AddToCollisionTracker(reinterpret_cast<uintptr_t>(&obA), const_cast<btCollisionObject*>(obB));

		//code for future reference

		//int numContacts = contactManifold->getNumContacts();
		//for (int j = 0;j<numContacts;j++)
		//{
		//	btManifoldPoint& pt = contactManifold->getContactPoint(j);
		//	if (pt.getDistance()<0.f)
		//	{
		//		const btVector3& ptA = pt.getPositionWorldOnA();
		//		const btVector3& ptB = pt.getPositionWorldOnB();
		//		const btVector3& normalOnB = pt.m_normalWorldOnB;
		//	}
		//}
	}
}

// detect collision,not efficient
void Physics::DetectCollisions()
{
	this->dynamicsWorld->performDiscreteCollisionDetection();

	int numManifolds = this->dynamicsWorld->getDispatcher()->getNumManifolds();

	for (int i = 0;i<numManifolds;i++)

	{

		btPersistentManifold* contactManifold = this->dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);

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