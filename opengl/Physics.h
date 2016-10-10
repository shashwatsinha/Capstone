#pragma once

#include "btBulletDynamicsCommon.h"
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "ActorFactory.h";

 class Physics
{
public:
	 std::unordered_map<int, std::vector<btCollisionObject*>> collisionTracker;

	Physics();
	~Physics();

	bool ExistsAlreadyInCollisionTracker(int nativeObjKey);

	void AddToCollisionTracker(int nativeid, btCollisionObject * colloidedObj);

	bool RemoveFromCollisionTracker(int nativeid, btCollisionObject * colloidedObj);

	bool RemoveFromCollisionTracker(int nativeid);

	void CreatePhysicsWorld();

	bool DetectCollision(btCollisionObject * target1, btCollisionObject * target2, Physics * physics);

	bool DetectCollision(btCollisionObject * target, Physics * physics);

	void DetectCollision();

	void DetectCollisions();


	btBroadphaseInterface* broadphase;

	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;

	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver;

	// The world.
	btDiscreteDynamicsWorld* dynamicsWorld;

	//for ground
	btCollisionShape* groundShape;
	btDefaultMotionState* groundMotionState;
	btRigidBody* groundRigidBody;

	static Physics *instance()
	{
		if (!phy_instance)
			phy_instance = new Physics;
		return phy_instance;
	}

private:
	static Physics *phy_instance;
};

