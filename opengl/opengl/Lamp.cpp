#include "Lamp.h"


void* load(char * fname, long * bufsize);

Lamp::Lamp()
{
	//srand(time(NULL));

	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1.);
	alSourcef(source, AL_GAIN, 1.5f);
	alSource3f(source, AL_POSITION, this->GetPosition().x, this->GetPosition().y, this->GetPosition().z);
	alSource3f(source, AL_VELOCITY, 0., 0., 0.);
	alSourcei(source, AL_LOOPING, AL_TRUE);

	alGenBuffers(1, &buffer);
	{
		long dataSize;
		const ALvoid* data = load("Lamp.raw", &dataSize);
		/* for simplicity, assume raw file is signed-16b at 44.1kHz */
		alBufferData(buffer, AL_FORMAT_MONO16, data, dataSize, 44100);
		free((void*)data);
	}

	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);
}

void Lamp::Render(Shader * shader)
{
	Draw(shader);

	if (currentYPos - meanPosition.y > 0.5f)
	{
		dirAmplitude = -0.01f;
	}

	if (currentYPos - meanPosition.y < -0.5f)
	{
		dirAmplitude = 0.01f;
	}

	currentYPos = currentYPos + dirAmplitude;
	this->SetPosition(glm::vec3(this->GetPosition().x, currentYPos, this->GetPosition().z));

	alSource3f(source, AL_POSITION, this->GetPosition().x, this->GetPosition().y, this->GetPosition().z);
}

void Lamp::SetInitialMeanPosition()
{
	meanPosition = this->GetPosition();
	currentYPos = meanPosition.y;
	int val = static_cast<int>(glfwGetTime());;
	//float randValue = rand() % 10 + 1;
	if(val%2==0)
		dirAmplitude = 0.1f;
	else
		dirAmplitude = -0.1f;
}


Lamp::~Lamp()
{
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
}
