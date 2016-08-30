#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"
#include "Mesh.h"
#include <SOIL.h>
#include "MemoryMacros.h"
#include "MemoryPool.h"
using namespace std;
// GL Includes
#include <GL/glew.h> // Contains all the necessery OpenGL includes
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class Model 
{
public:
	/*  Functions   */
	Model(GLchar* path)
	{
		this->loadModel(path);
	}
	Model();
	void Draw(Shader *shader);

	void SetPosition(GLfloat x, GLfloat y, GLfloat z);
	void SetScale(GLfloat x, GLfloat y, GLfloat z);
	void SetPath(GLfloat x, GLfloat y, GLfloat z);
	void Load(string p);

private:
	/*  Model Data  */
	vector<Mesh> meshes;
	string directory;
	GLfloat positionX = 0.0f;
	GLfloat positionY = 0.0f;
	GLfloat positionZ = 0.0f;
	GLfloat scaleX;
	GLfloat scaleY;
	GLfloat scaleZ;
	vector<Texture> textures_loaded;
	/*  Functions   */
	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,string typeName);
	GLint TextureFromFile(const char* path, string directory);

	GCC_MEMORYPOOL_DECLARATION(0);
};