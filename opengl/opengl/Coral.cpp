#include "Coral.h"



Coral::Coral()
{
	lerpColor = false;


}


Coral::~Coral()
{
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
}

float Coral::DistanceFromPlayer(glm::vec3 playerPos)
{
	float x = playerPos.x - this->GetPosition().x;
	float y = playerPos.y - this->GetPosition().y;
	float z = playerPos.z - this->GetPosition().z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

void Coral::IsParticleActivated(glm::vec3 pos)
{
	playerPos = pos;
	if (DistanceFromPlayer(playerPos) > 350)
	{
		emitParticles = false;
	}

	else
	{
		emitParticles = true;
		lerpColor = true;
	}
	alSource3f(source, AL_POSITION, GetPosition().x, GetPosition().y, GetPosition().z);
}

bool Coral::ActivateParticles()
{
	return emitParticles;
}

void Coral::SetLerpColorStatus(bool status)
{
	lerpColor = status;
}

void Coral::InitializeSound()
{
	alGenSources(1, &source);
	alSourcef(source, AL_PITCH, 1.);
	alSourcef(source, AL_GAIN, 0.8f);
	alSource3f(source, AL_POSITION, this->GetPosition().x, this->GetPosition().y, this->GetPosition().z);
	alSource3f(source, AL_VELOCITY, 0., 0., 0.);
	alSourcei(source, AL_LOOPING, AL_TRUE);

	alGenBuffers(1, &buffer);
	{
		long dataSize;
		const ALvoid* data = load("Coral_Object.raw", &dataSize);
		/* for simplicity, assume raw file is signed-16b at 44.1kHz */
		alBufferData(buffer, AL_FORMAT_MONO16, data, dataSize, 44100);
		free((void*)data);
	}

	alSourcei(source, AL_BUFFER, buffer);
	alSourcePlay(source);

	fflush(stderr); /* in case OpenAL reported an error earlier */
}

void * Coral::load(char * fname, long * bufsize)
{
	FILE* fp = fopen(fname, "rb");
	fseek(fp, 0L, SEEK_END);
	long len = ftell(fp);
	rewind(fp);
	void *buf = malloc(len);
	fread(buf, 1, len, fp);
	fclose(fp);
	*bufsize = len;
	return buf;
}

bool Coral::GetLerpColorStatus()
{
	return lerpColor;
}
