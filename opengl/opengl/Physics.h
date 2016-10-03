#pragma once

#define _XM_NO_INTRINSICS_ 1;

#include "btBulletDynamicsCommon.h"
#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include "ActorFactory.h";

static class Physics
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

	bool DetectCollision(btCollisionObject * target1, btCollisionObject * target2, Physics * physics, std::vector<ActorFactory*> physicsGameObjects);

	bool DetectCollision(btCollisionObject * target, Physics * physics);

	void DetectCollision(Physics * physics, std::vector<ActorFactory*> physicsGameObjects);

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
};

