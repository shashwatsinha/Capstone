#define GLEW_STATIC
#include <al.h>
#include <alc.h>
#define NUM_BUFFERS 3
#define BUFFER_SIZE 4096
#include "Game.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "SOIL.lib")
#pragma comment(lib, "assimp-vc130-mt.lib")

#include <iostream>

#include "Framework.h"
#define	TEST_WAVE_FILE		"Footsteps.wav"

GLuint screenWidth = 800, screenHeight = 600;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void InitGLFW();
double calcFPS(double theTimeInterval, string theWindowTitle);

GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLFWwindow* window;

string windowTitle = "OurGame";

Game game(screenWidth, screenHeight);

bool isVR = false;

static void list_audio_devices(const ALCchar *devices)
{
	const ALCchar *device = devices, *next = devices + 1;
	size_t len = 0;

	fprintf(stdout, "Devices list:\n");
	fprintf(stdout, "----------\n");
	while (device && *device != '\0' && next && *next != '\0') {
		fprintf(stdout, "%s\n", device);
		len = strlen(device);
		device += (len + 1);
		next += (len + 2);
	}
	fprintf(stdout, "----------\n");
}

void* load(char *fname, long *bufsize) {
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

int main(int argc, char **argv)
{

	
	/* initialize OpenAL context, asking for 44.1kHz to match HRIR data */
	ALCint contextAttr[] = { ALC_FREQUENCY,44100,0 };
	ALCdevice* device = alcOpenDevice(NULL);
	ALCcontext* context = alcCreateContext(device, contextAttr);
	alcMakeContextCurrent(context);

	
	alListener3f(AL_POSITION, Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y, Camera::instance()->GetPosition().z);

	//ALuint source;
	//alGenSources(1, &source);
	//alSourcef(source, AL_PITCH, 1.);
	//alSourcef(source, AL_GAIN, 5);
	//alSource3f(source, AL_POSITION, -250, 0, 50);
	//alSource3f(source, AL_VELOCITY, 0., 0., 0.);
	//alSourcei(source, AL_LOOPING, AL_TRUE);

	///* allocate an OpenAL buffer and fill it with monaural sample data */
	//ALuint buffer;
	//alGenBuffers(1, &buffer);
	//{
	//	long dataSize;
	//	const ALvoid* data = load("footsteps.raw", &dataSize);
	//	/* for simplicity, assume raw file is signed-16b at 44.1kHz */
	//	alBufferData(buffer, AL_FORMAT_MONO16, data, dataSize, 44100);
	//	free((void*)data);
	//}
	//alSourcei(source, AL_BUFFER, buffer);

	//alSourcePlay(source);

	//fflush(stderr); /* in case OpenAL reported an error earlier */


	// Start Game within Menu State
	game.State = GAME_ACTIVE;

	// Init GLFW
	InitGLFW();

	Camera::instance()->InitValues();
	srand(time(NULL));

	if (isVR)
	{
		//set the window for VR
		game.window = window;
		game.windowTitle = windowTitle;

		// Initializes LibOVR, and the Rift
		ovrResult result = ovr_Initialize(nullptr);
		VALIDATE(OVR_SUCCESS(result), "Failed to initialize libOVR.");
		//Platform.Window = GetForegroundWindow();

		//VALIDATE(Platform.InitWindow(GetModuleHandle(NULL), L"Oculus Room Tiny (GL)"), "Failed to open window.");
		game.isVR = true;
		game.RunVR();
		ResourceManager::Clear();

		glfwTerminate();
		return 0;

		ovr_Shutdown();

	}

	else
	{

		
		// Initialize game
		game.Init();

		while (!glfwWindowShouldClose(window))
		{
			//ALFWprintf(".");
			// Get Source State
			
			calcFPS(1.0, windowTitle);

			//alSource3f(source, AL_POSITION, -250, 0, 50);
			alListener3f(AL_POSITION, Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y, Camera::instance()->GetPosition().z);

			// Set frame time
			GLfloat currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

			// Check and call events
			glfwPollEvents();

			// Manage user input
			game.ProcessInput(deltaTime);

			// Update Game state
			game.Update(deltaTime);

			// Render
			game.Render();

			glfwSwapBuffers(window);
		}
	}

	ResourceManager::Clear();
	game.CleanUp();

	/*alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);*/
	alcDestroyContext(context);
	alcCloseDevice(device);

	glfwTerminate();
	return 0;
}



// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			game.Keys[key] = GL_TRUE;
		else if (action == GLFW_RELEASE)
			game.Keys[key] = GL_FALSE;
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	Camera::instance()->ProcessMouseMovement(xoffset, yoffset, true);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	Camera::instance()->ProcessMouseScroll(yoffset);
}

#pragma endregion


void InitGLFW()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	if (isVR)
	{
		screenWidth = 1000;
	}

	window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, screenWidth, screenHeight);

	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

}

double calcFPS(double theTimeInterval, string theWindowTitle)
{
	// Static values which only get initialised the first time the function runs
	static double t0Value = glfwGetTime(); // Set the initial time to now
	static int    fpsFrameCount = 0;             // Set the initial FPS frame count to 0
	static double fps = 0.0;           // Set the initial FPS value to 0.0

									   // Get the current time in seconds since the program started (non-static, so executed every time)
	double currentTime = glfwGetTime();

	// Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s)
	// Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least.
	if (theTimeInterval < 0.1)
	{
		theTimeInterval = 0.1;
	}
	if (theTimeInterval > 10.0)
	{
		theTimeInterval = 10.0;
	}

	// Calculate and display the FPS every specified time interval
	if ((currentTime - t0Value) > theTimeInterval)
	{
		// Calculate the FPS as the number of frames divided by the interval in seconds
		fps = (double)fpsFrameCount / (currentTime - t0Value);

		// If the user specified a window title to append the FPS value to...
		if (theWindowTitle != "NONE")
		{
			// Convert the fps value into a string using an output stringstream
			std::ostringstream stream;
			stream << fps;
			std::string fpsString = stream.str();

			// Append the FPS value to the window title details
			theWindowTitle += " | FPS: " + fpsString;

			// Convert the new window title to a c_str and set it
			const char* pszConstString = theWindowTitle.c_str();
			glfwSetWindowTitle(window, pszConstString);
		}
		else // If the user didn't specify a window to append the FPS to then output the FPS to the console
		{
			std::cout << "FPS: " << fps << std::endl;
		}

		// Reset the FPS frame counter and set the initial time to be now
		fpsFrameCount = 0;
		t0Value = glfwGetTime();
	}
	else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter
	{
		fpsFrameCount++;
	}

	// Return the current FPS - doesn't have to be used if you don't want it!
	return fps;
}


