#pragma once
// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Mesh.h"

//bullet physics
#include "btBulletDynamicsCommon.h"
#include"Physics.h"

GLint TextureFromFile(const char* path, string directory);

class Model
{
public:
	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model();

	void InitPath(GLchar* path);
	
	// Draws the model, and thus all its meshes
	void Draw(Shader *shader);

	//Set the position of the model
	void SetPosition(glm::vec3);

	//Get the position of the model
	glm::vec3 GetPosition();

	//Set the scale of the model
	void SetScale(glm::vec3);

	//Get the scale of the model
	glm::vec3 GetScale();

	//virtual function
	virtual void ProcessAI() {};


	//create a dynamic rigidbody
	btAlignedObjectArray<btCollisionShape*> collisionShapes;
	btCollisionShape* colShape;
	// Create Dynamic Objects
	btTransform  startTransform;
	float mass;
	btVector3 localInertia;
	btDefaultMotionState* myMotionState;
	btRigidBody* body;
	btBoxShape * mPlayerBox;
	btCollisionObject * mPlayerObject;
	int GameObjID;

	void InitializeRigidBody();
	void SetMass(float newmass);
	void SetDefaultMass();
	void CleanupPhysicsObjects();
	void SetRigidBodyShape(float scalex, float scaley, float scalez);
	/*----------------------physics end----------------------*/
	//XMFLOAT4X4 GetWorldMatrix() {
	//	return worldMatrix;
	//}


	
private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.

										/*  Functions   */
										// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
	void loadModel(string path);
	

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void processNode(aiNode* node, const aiScene* scene);

	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);

	//Position of the Model
	glm::vec3 position;

	//Scale of the model
	glm::vec3 scale;

	
	
};




