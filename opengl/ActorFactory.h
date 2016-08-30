#pragma once
#include <map>
#include <iostream>
#include <string>
#include "Model.h"
#include "Resource.h"
#include "Source\3rdPartyTools\tinyxml.h"
using namespace std;
class ActorFactory : public Resource
{
public:
	Model *ourModel;
	Shader *shader1;
	unsigned long actorID;
	map<string, string> ActorComponentCreatorMap;
	void ShaderInitialization();
	void ShaderInput(Shader *shader);
	ActorFactory();
	~ActorFactory();
	void VOnInit(const char * resourceString);
	void VOnUpdate(unsigned long deltaMs);
};

