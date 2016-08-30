#include "ActorFactory.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void ActorFactory::VOnInit(const char * resourceString)
{
	TiXmlDocument doc(resourceString);
	bool loadOkay = doc.LoadFile();
	TiXmlElement * root = doc.FirstChildElement();

	for (TiXmlElement* pNode = root->FirstChildElement(); pNode; pNode = pNode->NextSiblingElement())
	{
		ActorComponentCreatorMap[pNode->Value()] = pNode->GetText();
		cout << pNode->Value() << " " << pNode->GetText() << endl;
	}
	ourModel = new Model();
	ourModel->SetPosition(0, -10, 0);
	ourModel->SetScale(stof(ActorComponentCreatorMap["InitialScale"]), stof(ActorComponentCreatorMap["InitialScale"]), stof(ActorComponentCreatorMap["InitialScale"]));
	ourModel->Load(ActorComponentCreatorMap["Path"]);
	ShaderInitialization();

}

void ActorFactory::ShaderInitialization()
{
	shader1 = new Shader();
	shader1->Init("Shaders/nanosuit.vs", "Shaders/nanosuit.frag");
}

void ActorFactory::ShaderInput(Shader *shader)
{
	GLuint screenWidth = 800, screenHeight = 600;
	Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
	// Clear the colorbuffer
	glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	shader->Use();
	// <-- Don't forget this one!
	// Transformation matrices
	glm::mat4 projection = glm::perspective(camera.Zoom, (float)screenWidth / (float)screenHeight, 0.1f, 100.0f);
	glm::mat4 view = camera.GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(glGetUniformLocation(shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

ActorFactory::ActorFactory()
{
}


ActorFactory::~ActorFactory()
{
}

void ActorFactory::VOnUpdate(unsigned long deltaMs) 
{
	ShaderInput(shader1);
	ourModel->Draw(shader1);
}
