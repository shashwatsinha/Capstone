#include "Game.h"
//#include "resource_manager.h"

float bgX = 0.0f;
float bgY = 0.0f;
float bgZ = 0.0f;
Game::Game(GLuint width, GLuint height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{

	

	glDeleteVertexArrays(1, &quadVAO);
	glDeleteBuffers(1, &quadVBO);
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteRenderbuffers(1, &rbo);
}

void Game::Init()
{
	flock1 = new Flockers();
	flock2 = new Flockers();
	flock3 = new Flockers();
	planet = new Model();
	sphere = new Model();
	frustum = new Frustum();

	theta = 0.0f;
	seperator = 1;
	centreOfFlock1 = glm::vec3(-250, 0, 50);
	centreOfFlock2 = glm::vec3(150, -100, 50);
	
	

	flock1->InitializeFlock(27, centreOfFlock1, 1.0f, false, 0.0f, true, 2);
	flock2->InitializeFlock(27, centreOfFlock2, 1.0f,false, 0.0f, true, 2);
	flock3->InitializeFlock(27, glm::vec3(350, 0, 250), 1.0f, false, 0.0f, true, 2);
	
	centreOfFlock3 = glm::vec3(-50, 0, 50);
	
	
	
	for (int i = 1; i < 2; i++)
	{
		Satellite *s = new Satellite(2);
		s->InitPath("Models/GreenObject/GreenObject.obj");
		s->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
		s->SetPosition(glm::vec3(0, 0, 0));
		satellites.push_back(s);
	}

	vector<glm::vec3> coralPositions;
	//coral positions
	glm::vec3 coralPosition1 = glm::vec3(-21.25,-18.75,135.5); coralPositions.push_back(coralPosition1);
	glm::vec3 coralPosition2 = glm::vec3(-27,-16.25,120.5); coralPositions.push_back(coralPosition2); 
	glm::vec3 coralPosition3 = glm::vec3(-27,-11.25,111.25); coralPositions.push_back(coralPosition3);
	glm::vec3 coralPosition4 = glm::vec3(-17.75,-11,111.25); coralPositions.push_back(coralPosition4);
	glm::vec3 coralPosition5 = glm::vec3(-17.25,-10.75,98.75); coralPositions.push_back(coralPosition5); 
	glm::vec3 coralPosition6 = glm::vec3(-20.75,-8.25,89.25); coralPositions.push_back(coralPosition6);
	glm::vec3 coralPosition7 = glm::vec3(-35.5,-4.5,89.25); coralPositions.push_back(coralPosition7);
	glm::vec3 coralPosition8 = glm::vec3(-40.5,-15.75,123.25); coralPositions.push_back(coralPosition8);
	glm::vec3 coralPosition9 = glm::vec3(-28,-17.25,136.5); coralPositions.push_back(coralPosition9);
	glm::vec3 coralPosition10 = glm::vec3(-35.25,-16.25,145.25); coralPositions.push_back(coralPosition10);

	//surface emitter positions
	vector<glm::vec3> surfaceEmitterPositions;
	glm::vec3 surfaceEmitterPosition1 = glm::vec3(-21,13.25,61); surfaceEmitterPositions.push_back(surfaceEmitterPosition1);
	glm::vec3 surfaceEmitterPosition2 = glm::vec3(-21,24.75,60.75); surfaceEmitterPositions.push_back(surfaceEmitterPosition2);
	glm::vec3 surfaceEmitterPosition3 = glm::vec3(-32.5,9.75,60.75); surfaceEmitterPositions.push_back(surfaceEmitterPosition3);
	glm::vec3 surfaceEmitterPosition4 = glm::vec3(-40.25,13.5,61); surfaceEmitterPositions.push_back(surfaceEmitterPosition4);
	glm::vec3 surfaceEmitterPosition5 = glm::vec3(-3.75,13.5,61); surfaceEmitterPositions.push_back(surfaceEmitterPosition5);


	// Load Default Shader
	ResourceManager::LoadShader("Shaders/vertexShader_default.vs", "Shaders/fragmentShader_default.frag", nullptr, "default");

	planet->InitPath("Models/Planet/planet.obj");
	planet->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	planet->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 10; k++)
			{
				EnvironmentObject *obj = new EnvironmentObject();
				obj->InitPath("Models/Bullet/Bullet.obj");
				obj->SetPosition(glm::vec3((i * 4) - 30 , (j * 4)-10, k * 3 + 200));
				obj->SetScale(glm::vec3(4.0f, 4.0f, 4.0f));
				bgObjs.push_back(obj);
			}
		}
	}

	ResourceManager::LoadShader("Shaders/vertexShader_Coral.vs", "Shaders/fragmentShader_Coral.frag", nullptr, "coralShader");
	
	Coral *masterCoral = new Coral();
	masterCoral->InitPath("Models/Corals/coral0.obj");
	masterCoral->SetScale(glm::vec3(0.3f, 0.3f, 0.3f));


	for (int i = 0;i < coralPositions.size();i++)
	{
		Coral * coral = new Coral();
		*coral = *masterCoral;
		coral->SetPosition(coralPositions[i]);
		corals.push_back(coral);
	}

	//sample coral
	coral1 = new Coral();
	coral1->InitPath("Models/Corals/coral0.obj");
	coral1->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));
	



	// Load Skybox Shader
	//ResourceManager::LoadShader("Shaders/vertexShader_Skybox.vs", "Shaders/fragmentShader_Skybox.frag", nullptr, "skybox");
	//skybox.setupMesh();

	// Setup Directional Light
	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight = new Lights();
	directionalLight->initializeDirectionalLightParameters(direction, ambient, diffuse, specular);

	// Initialize Point Light (Initialize as many point lights as requires and push the lights into the pointLights vector)
	// Cuurently the max number of point lights is set as 10 in fragment shader. For more lights change the value in fragment shader
	glm::vec3 pointLightPosition1 = glm::vec3(1.0, 150.5, 23.75); pointLightPositions.push_back(pointLightPosition1);
	/*glm::vec3 pointLightPosition2 = glm::vec3(1.0, 148.0, 44.0); pointLightPositions.push_back(pointLightPosition2);
	glm::vec3 pointLightPosition3 = glm::vec3(-9.5, 148.75, 33.75); pointLightPositions.push_back(pointLightPosition3);
	glm::vec3 pointLightPosition4 = glm::vec3(12.25, 148.75, 33.75); pointLightPositions.push_back(pointLightPosition4);
	glm::vec3 pointLightPosition5 = glm::vec3(-9.0, 143.5, 54.25); pointLightPositions.push_back(pointLightPosition5);
	glm::vec3 pointLightPosition6 = glm::vec3(13.0, 140.25, 54.25); pointLightPositions.push_back(pointLightPosition6);
	glm::vec3 pointLightPosition7 = glm::vec3(0.75, 150.0, 4.25); pointLightPositions.push_back(pointLightPosition7);
	glm::vec3 pointLightPosition8 = glm::vec3(-10.25, 151.5, 14.0); pointLightPositions.push_back(pointLightPosition8);
	glm::vec3 pointLightPosition9 = glm::vec3(12.0, 150.0, 14.0); pointLightPositions.push_back(pointLightPosition9);
	glm::vec3 pointLightPosition10 = glm::vec3(12.0, 147.75, -8.0); pointLightPositions.push_back(pointLightPosition10);
	glm::vec3 pointLightPosition11 = glm::vec3(-11.25, 148.25, -8.0); pointLightPositions.push_back(pointLightPosition11);
	glm::vec3 pointLightPosition12 = glm::vec3(0.2, 149.0, -18.0); pointLightPositions.push_back(pointLightPosition12);
	glm::vec3 pointLightPosition13 = glm::vec3(-11.8, 147.75, -28.75); pointLightPositions.push_back(pointLightPosition13);
	glm::vec3 pointLightPosition14 = glm::vec3(11.2, 147.75, -28.75); pointLightPositions.push_back(pointLightPosition14);*/
	//  vec3(11.200000, 147.750000, -28.750000)

	// Setup Point Light. Properties of Point Light can be changed over time if required. (In Game loop change the values if required)
	// Get the Point Light whose values need to be changed using the vector pointLights and change the properties as required
	ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	specular = glm::vec3(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < pointLightPositions.size(); i++)
	{
		Lights *pointLight = new Lights();
		// Note that the 4th parameter is the distance the point light should affect (set the distance from the pre initialized pointLightDistance array)
		pointLight->setPointLightParameters(pointLightPositions[i], ambient, diffuse, specular, pointLightDistance[4]);
		pointLights.push_back(pointLight);
	}

	// Point Light Container Shader
	ResourceManager::LoadShader("Shaders/vertexShader_LightContainer.vs", "Shaders/fragmentShader_LightContainer.frag", nullptr, "lightContainerShader");
	for (int i = 0; i < pointLightPositions.size(); i++)
	{
		Model *pointLightContainer = new Model();
		pointLightContainer->InitPath("Models/Bullet/Bullet.obj");
		pointLightContainer->SetPosition(pointLightPositions[i]);
		pointLightContainer->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
		pointLightContainers.push_back(pointLightContainer);
	}

	ResourceManager::LoadShader("Shaders/vertexShader_Skysphere.vs", "Shaders/fragmentShader_Skysphere.frag", nullptr, "skySphere");
	sphere->InitPath("Models/Sphere/perfect_sphere.obj");
	sphere->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	sphere->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	//loading resources
	ResourceManager::LoadShader("Shaders/particle.vs", "Shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadTexture("Textures/whitesnowflake.png", GL_TRUE, "particle");
	ResourceManager::LoadTexture("Textures/star.png", GL_TRUE, "coralParticle");
	particlesystem1 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 5000);
	particlesystem1->age = 10.0f;
	particlesystem1->initialPosition = glm::vec3(0.5, 0.5, 370.0);
	particlesystem1->acceleration = glm::vec3(0.0, 0.0, 0.5);
	particlesystem1->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	particlesystem1->startVelocityMin = 1.f;
	particlesystem1->startVelocityRange = 1.5f;
	particlesystem1->scale = 0.1f;

	//coral particle system

	ParticleSystem *masterCoralParticle = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("coralParticle"), 1000);
	masterCoralParticle->age = 4.0f;
	masterCoralParticle->acceleration = glm::vec3(0.0, 1.5, 0.0);
	masterCoralParticle->color = glm::vec4(0.1f, 0.8f, 0.4f, 1.0f);
	masterCoralParticle->startVelocityMin = 0.1f;
	masterCoralParticle->startVelocityRange = 0.1f;
	masterCoralParticle->scale = 0.1f;

	for (int i = 0;i < coralPositions.size();i++)
	{
		ParticleSystem *coralParticle = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
		*coralParticle = *masterCoralParticle;
		coralParticle->initialPosition = coralPositions[i];
		coralParticles.push_back(coralParticle);
	}

	//surface emitter setup
	ParticleSystem* masterSurfaceEmitter = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 5000);
	masterSurfaceEmitter->age = 15.0f;
	masterSurfaceEmitter->acceleration = glm::vec3(0.0, 0.0, 1.5);
	masterSurfaceEmitter->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	masterSurfaceEmitter->startVelocityMin = 1.1f;
	masterSurfaceEmitter->startVelocityRange = 1.5f;
	masterSurfaceEmitter->scale = 0.8f;

	for (int i = 0;i < surfaceEmitterPositions.size();i++)
	{
		ParticleSystem *surfaceParticle = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
		*surfaceParticle = *masterSurfaceEmitter;
		surfaceParticle->initialPosition = surfaceEmitterPositions[i];
		surfaceEmitter.push_back(surfaceParticle);
	}

	// Initialize framebuffer and setup the screenQuad
	setupScreenQuadAndFrameBuffer();
}

void Game::Update(GLfloat dt)
{
	
	
	//coral particle update
	for (int i = 0;i < coralParticles.size();i++)
	{
		coralParticles[i]->Update(dt, 2);
	}

	for (int i = 0;i < surfaceEmitter.size(); i++)
	{
		surfaceEmitter[i]->Update(dt, 2);
	}
}


void Game::ProcessInput(GLfloat dt)
{
	if (this->State == GAME_ACTIVE)
	{
		// Camera controls
		if (this->Keys[GLFW_KEY_W])
			Camera::instance()->ProcessKeyboard(FORWARD, dt);
		if (this->Keys[GLFW_KEY_S])
			Camera::instance()->ProcessKeyboard(BACKWARD, dt);
		if (this->Keys[GLFW_KEY_A])
			Camera::instance()->ProcessKeyboard(LEFT, dt);
		if (this->Keys[GLFW_KEY_D])
			Camera::instance()->ProcessKeyboard(RIGHT, dt);

		if (Camera::instance()->GetPosition().z > 61 && Camera::instance()->GetPosition().z < 200 &&
			Camera::instance()->GetPosition().x > -100 && Camera::instance()->GetPosition().x < 10 &&
			Camera::instance()->GetPosition().y > -25 && Camera::instance()->GetPosition().y < 35)
		{
			Camera::instance()->MovePositiveZ(dt);
			Camera::instance()->SetSpeed(8.0f);
		}

		else
		{
			Camera::instance()->SetSpeed(10.0f);
		}

		if (this->Keys[GLFW_KEY_I])
		{
			bgY += 0.25;
		}

		if (this->Keys[GLFW_KEY_I])
		{
			pointLightPositions[pointLightPositions.size()-1].y += .25;
			coral1Position.y += .25;

		}
		if (this->Keys[GLFW_KEY_K])
		{
			pointLightPositions[pointLightPositions.size() - 1].y -= .25;
			coral1Position.y -= .25;
		}
		if (this->Keys[GLFW_KEY_J])
		{
			pointLightPositions[pointLightPositions.size() - 1].z += .25;
			coral1Position.z += .25;
		}
		if (this->Keys[GLFW_KEY_L])
		{
			pointLightPositions[pointLightPositions.size() - 1].z -= .25;
			coral1Position.z -= .25;
		}
		if (this->Keys[GLFW_KEY_U])
		{
			pointLightPositions[pointLightPositions.size() - 1].x += .25;
			coral1Position.x += .25;
		}
		if (this->Keys[GLFW_KEY_O])
		{
			pointLightPositions[pointLightPositions.size() - 1].x -= .25;
			coral1Position.x -= .25;
		}
		if (this->Keys[GLFW_KEY_P])
		{
			//cout << "Position : " + glm::to_string(pointLightPositions[pointLightPositions.size() - 1]);
			cout << "position:" << coral1Position.x<<"," << coral1Position.y<<"," << coral1Position.z << endl;
		}
	}
}

//void Game::RenderVR()
//{
//	glm::mat4 view = Camera::instance()->GetViewMatrix();
//	glm::mat4 projection = glm::perspective(Camera::instance()->Zoom, static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 100.0f);
//
//	Shader shader = ResourceManager::GetShader("default");
//	shader.Use();
//	shader.SetMatrix4("view", view);
//	shader.SetMatrix4("projection", projection);
//
//	// Setup Directional Light
//	directionalLight.setUpDirectionalLight(&ResourceManager::GetShader("default"), Camera::instance());
//
//	// Setup Point Light. Properties of Point Light can be changed over time if required.
//	// Get the Point Light whose values need to be changed using the vector pointLights and change the properties as required
//	glm::vec3 position = glm::vec3(sin(glfwGetTime() * 1.0f), 0.0f, cos(glfwGetTime() * 1.0f));
//	glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
//	glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
//	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
//	float constant = 1.0f;
//	float linear = 0.09f;
//	float quadratic = 0.032f;
//	pointLightContainer.SetPosition(glm::vec3(sin(glfwGetTime() * 1.0f), 0.0f, cos(glfwGetTime() * 1.0f)));
//	for (int i = 0; i < pointLights.size(); i++)
//	{
//		// Note that the 4th parameter is the distance the point light should affect (set the distance from the pre initialized pointLightDistance array)
//		pointLights[i].setPointLightParameters(position, ambient, diffuse, specular, pointLightDistance[4]);
//		pointLights[i].setUpPointLight(&ResourceManager::GetShader("default"), Camera::instance());
//	}
//	ResourceManager::GetShader("default").SetInteger("NO_OF_POINT_LIGHTS", pointLights.size());
//
//	glm::quat myQuat;
//	glm::quat key_quat = glm::quat(glm::vec3(0.0f, (GLfloat)glfwGetTime() * glm::radians(20.0f), 0.0f));
//	myQuat = key_quat * myQuat;
//	myQuat = glm::normalize(myQuat);
//	//spaceShip.SetRotation(myQuat);
//	spaceShip.DrawVR(&shader);
//
//	planet.DrawVR(&shader);
//
//	Shader coralShader = ResourceManager::GetShader("coralShader");
//	coralShader.Use();
//	coralShader.SetMatrix4("view", view);
//	coralShader.SetMatrix4("projection", projection);
//
//	for (int i = 0; i < corals.size(); i++)
//	{
//		if (corals[i]->GetLerpColorStatus() == false)
//		{
//			mixValue = 0.0f;
//			coralShader.SetFloat("mixValue", mixValue);
//		}
//		else
//		{
//			mixValue = mixValue + (glfwGetTime() * 0.009f);
//			if (mixValue > 1.0f)
//				mixValue = 1.0f;
//			coralShader.SetFloat("mixValue", mixValue);
//		}
//		corals[i]->DrawVR(&coralShader);
//		corals[i]->IsParticleActivated(glm::vec3(Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y, Camera::instance()->GetPosition().z));
//		coralParticles[i]->ActivateParticles(corals[i]->ActivateParticles());
//	}
//
//	Shader particleShader = ResourceManager::GetShader("particle");
//	particleShader.Use();
//	view = Camera::instance()->GetViewMatrix();
//	particleShader.SetMatrix4("view", view);
//	particleShader.SetMatrix4("projection", projection);
//	glm::mat4 model;
//	//model = glm::translate(model, particle.Position);
//	model[0][0] = view[0][0];
//	model[0][1] = view[1][0];
//	model[0][2] = view[2][0];
//	model[1][0] = view[0][1];
//	model[1][1] = view[1][1];
//	model[1][2] = view[2][1];
//	model[2][0] = view[0][2];
//	model[2][1] = view[1][2];
//	model[2][2] = view[2][2];
//	particleShader.SetMatrix4("model", model);
//
//	particlesystem1->DrawVR();
//	particlesystem2->DrawVR();
//
//	for (int i = 0; i < physicsObjects.size(); i++)
//	{
//		physicsObjects[i]->DrawVR(&ResourceManager::GetShader("default"));
//		for (int j = 0; j < physicsObjects[i]->enemyBullets.size(); j++)
//		{
//			physicsObjects[i]->enemyBullets[j]->DrawVR(&ResourceManager::GetShader("default"));
//		}
//	}
//
//	for (int i = 0; i < bullets.size(); i++)
//	{
//		bullets[i]->DrawVR(&ResourceManager::GetShader("default"));
//	}
//
//	// Also draw the point light object, again binding the appropriate shader
//	Shader lampShader = ResourceManager::GetShader("lightContainerShader");
//	lampShader.Use();
//	lampShader.SetMatrix4("view", view);
//	lampShader.SetMatrix4("projection", projection);
//	pointLightContainer.DrawVR(&lampShader);
//
//	glDepthFunc(GL_LEQUAL);
//	Shader skySphereShader = ResourceManager::GetShader("skySphere");
//	skySphereShader.Use();
//	view = glm::mat4(glm::mat3(Camera::instance()->GetViewMatrix()));	// Remove any translation component of the view matrix
//	skySphereShader.SetMatrix4("view", view);
//	skySphereShader.SetMatrix4("projection", projection);
//	GLfloat timeValue = glfwGetTime();
//	skySphereShader.SetFloat("iGlobalTime", timeValue);
//	sphere.DrawVR(&skySphereShader);
//	glDepthFunc(GL_LESS);
//
//	// Draw the skybox last
//	//glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
//	//Shader skyboxShader = ResourceManager::GetShader("skybox");
//	//skyboxShader.Use();
//	//view = glm::mat4(glm::mat3(Camera::instance()->GetViewMatrix()));	// Remove any translation component of the view matrix
//	//skyboxShader.SetMatrix4("view", view);
//	//skyboxShader.SetMatrix4("projection", projection);
//	//skybox.Draw(&skyboxShader);
//	//glDepthFunc(GL_LESS); // Set depth function back to default
//
//	DetectCollisions();
//}

void Game::Render()
{
	/////////////////////////////////////////////////////
	// Bind to framebuffer and draw to color texture 
	// as we normally would.
	// //////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Clear all attached buffers        
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // We're not using stencil buffer so why bother with clearing?

	glEnable(GL_DEPTH_TEST);
	
	
	if (isVR)
	{
		 camView = Camera::instance()->GetViewMatrix();
		 camProjection = glm::perspective(Camera::instance()->Zoom, static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 5000.0f);
	}
	
	else
	{
		 camView = Camera::instance()->GetViewMatrix();
		 camProjection = glm::perspective(Camera::instance()->Zoom, static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 400.0f);
	}

	frustum->ConstructFrustum(5, camProjection, camView);


	Shader shader = ResourceManager::GetShader("default");
	shader.Use();
	shader.SetMatrix4("view", camView);
	shader.SetMatrix4("projection", camProjection);
	
	// Setup Directional Light
	directionalLight->setUpDirectionalLight(&ResourceManager::GetShader("default"), Camera::instance());

	// Properties of Point Light can be changed over time if required here.
	// Get the Point Lights whose values need to be changed using the vector pointLights and change the properties as required
	glm::vec3 ambient = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	for (int i = 0; i < pointLightPositions.size(); i++)
	{
		if(i == (pointLightPositions.size() - 1))
			pointLights[i]->setPointLightParameters(pointLightPositions[i], ambient, diffuse, specular, pointLightDistance[4]);
		
		string *uniformName = new string[7];
		uniformName[0] = string("pointLights[") + to_string(i) + string("].position");
		uniformName[1] = string("pointLights[") + to_string(i) + string("].ambient");
		uniformName[2] = string("pointLights[") + to_string(i) + string("].diffuse");
		uniformName[3] = string("pointLights[") + to_string(i) + string("].specular");
		uniformName[4] = string("pointLights[") + to_string(i) + string("].constant");
		uniformName[5] = string("pointLights[") + to_string(i) + string("].linear");
		uniformName[6] = string("pointLights[") + to_string(i) + string("].quadratic");

		pointLights[i]->setUpPointLight(&ResourceManager::GetShader("default"), Camera::instance(), uniformName);
	}
	ResourceManager::GetShader("default").SetInteger("NO_OF_POINT_LIGHTS", pointLights.size());
	
	//glm::quat myQuat;
	//glm::quat key_quat = glm::quat(glm::vec3(0.0f, (GLfloat)glfwGetTime() * glm::radians(20.0f), 0.0f));
	//myQuat = key_quat * myQuat;
	//myQuat = glm::normalize(myQuat);
	//spaceShip.SetRotation(myQuat);
	//spaceShip.Draw(&shader);

//	cout << Camera::instance()->Front.x <<" "<< Camera::instance()->Front.y<<" " << Camera::instance()->Front.z << endl;
	glm::mat4 k = Camera::instance()->GetProjectionMatrix();
	planet->Draw(&shader);
	flock1->RenderFlock(&shader, Camera::instance()->GetPosition());
	flock2->RenderFlock(&shader, Camera::instance()->GetPosition());
	flock3->RenderFlock(&shader, Camera::instance()->GetPosition());
	//flock4->RenderFlock(&shader);
	//flock5->RenderFlock(&shader);
	
	for (int i = 0; i < bgObjs.size(); i++)
	{
		bgObjs[i]->Update(&shader, Camera::instance()->GetPosition());
	}

	
	for (int i = 0; i < satellites.size(); i++)
	{
		satellites[i]->UpdatePhysics();
		if(frustum->CheckSphere(satellites[i]->GetPosition(),0.1f))
			satellites[i]->Render(&shader);
	}

	//TestWithFlock();

	Shader coralShader = ResourceManager::GetShader("coralShader");
	coralShader.Use();
	coralShader.SetMatrix4("view", camView);
	coralShader.SetMatrix4("projection", camProjection);
	/*coral1->SetPosition(coral1Position);
	coral1->Draw(&coralShader);*/
	
	for (int i = 0;i < corals.size();i++)
	{
		if (corals[i]->GetLerpColorStatus() == false)
		{
			mixValue = 0.0f;
			coralShader.SetFloat("mixValue", mixValue);
		}
		else
		{
			mixValue = mixValue + (glfwGetTime() * 0.009f);
			if (mixValue > 1.0f)
				mixValue = 1.0f;
			coralShader.SetFloat("mixValue", mixValue);
		}
		corals[i]->Draw(&coralShader);
		corals[i]->IsParticleActivated(glm::vec3(Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y, Camera::instance()->GetPosition().z));
		coralParticles[i]->ActivateParticles(corals[i]->ActivateParticles());
	}

	// Also draw the point light object, again binding the appropriate shader
	/*Shader lampShader = ResourceManager::GetShader("lightContainerShader");
	lampShader.Use();
	lampShader.SetMatrix4("view", view);
	lampShader.SetMatrix4("projection", projection);
	for (int i = 0; i < pointLights.size(); i++)
	{
		if (i == (pointLightPositions.size() - 1))
			pointLightContainers[i]->SetPosition(pointLightPositions[i]);
		pointLightContainers[i]->Draw(&lampShader);
	}*/

	glDepthFunc(GL_LEQUAL);
	Shader skySphereShader = ResourceManager::GetShader("skySphere");
	skySphereShader.Use();
	camView = glm::mat4(glm::mat3(Camera::instance()->GetViewMatrix()));	// Remove any translation component of the view matrix
	skySphereShader.SetMatrix4("view", camView);
	skySphereShader.SetMatrix4("projection", camProjection);
	GLfloat timeValue = glfwGetTime();
	skySphereShader.SetFloat("iGlobalTime", timeValue);
	for (int i = 0; i < corals.size(); i++)
	{
		if (corals[i]->GetLerpColorStatus() == true)
			noOfCoralsActivated++;
	}
	if (noOfCoralsActivated == corals.size()) {
		noOfCoralsActivated = 0;
		skySphereShader.SetFloat("isColor", 1.0f);
	}
	else {
		noOfCoralsActivated = 0;
		skySphereShader.SetFloat("isColor", 0.0f);
	}
	sphere->Draw(&skySphereShader);
	glDepthFunc(GL_LESS);

	//add particles here

	Shader particleShader = ResourceManager::GetShader("particle");
	particleShader.Use();
	camView = Camera::instance()->GetViewMatrix();
	particleShader.SetMatrix4("view", camView);
	particleShader.SetMatrix4("projection", camProjection);
	glm::mat4 model;
	//model = glm::translate(model, particle.Position);
	model[0][0] = camView[0][0];
	model[0][1] = camView[1][0];
	model[0][2] = camView[2][0];
	model[1][0] = camView[0][1];
	model[1][1] = camView[1][1];
	model[1][2] = camView[2][1];
	model[2][0] = camView[0][2];
	model[2][1] = camView[1][2];
	model[2][2] = camView[2][2];
	particleShader.SetMatrix4("model", model);



	/*particlesystem1->SetModelMatrix();
	particlesystem1->SetMatrix("view", view);
	particlesystem1->SetMatrix("projection", projection);*/

	/*glm::mat4 model1;
	model1[0][0] = view[0][0];
	model1[0][1] = view[1][0];
	model1[0][2] = view[2][0];
	model1[1][0] = view[0][1];
	model1[1][1] = view[1][1];
	model1[1][2] = view[2][1];
	model1[2][0] = view[0][2];
	model1[2][1] = view[1][2];
	model1[2][2] = view[2][2];*/


	//particlesystem2->SetMatrix("view", view);
	//particlesystem2->SetMatrix("projection", projection);
	//particlesystem2->SetModelMatrix();
	//particlesystem2->SetMatrix("model", model1);



	//particlesystem1->Draw();
	
	//coral particle draw

	for (int i = 0;i < coralParticles.size();i++)
	{
		coralParticles[i]->Draw();
	}

	for (int i = 0;i < surfaceEmitter.size(); i++)
	{
		surfaceEmitter[i]->Draw();
	}

	
	// Draw the skybox last
	//glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	//Shader skyboxShader = ResourceManager::GetShader("skybox");
	//skyboxShader.Use();
	//view = glm::mat4(glm::mat3(Camera::instance()->GetViewMatrix()));	// Remove any translation component of the view matrix
	//skyboxShader.SetMatrix4("view", view);
	//skyboxShader.SetMatrix4("projection", projection);
	//skybox.Draw(&skyboxShader);
	//glDepthFunc(GL_LESS); // Set depth function back to default
	
	

	/////////////////////////////////////////////////////
	// Bind to default framebuffer again and draw the 
	// quad plane with attched screen texture.
	// //////////////////////////////////////////////////
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	// Clear all relevant buffers
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

	// Draw Screen
	Shader screenShader = ResourceManager::GetShader("screenShader");
	screenShader.Use();
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

// Other methods specific to our game

void Game::setupScreenQuadAndFrameBuffer()
{
	ResourceManager::LoadShader("Shaders/VertexShader_Quad.vs", "Shaders/FragmentShader_Quad.frag", nullptr, "screenShader");
	GLfloat quadVertices[] = {
		// Positions   // TexCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	// Setup screen VAO
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
	// Framebuffers
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// Create a color attachment texture
	textureColorbuffer = generateAttachmentTexture(false, false);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, this->Width, this->Height); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
																								  // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

// Generates a texture that is suited for attachments to a framebuffer
GLuint Game::generateAttachmentTexture(GLboolean depth, GLboolean stencil)
{
	// What enum to use?
	GLenum attachment_type;
	if (!depth && !stencil)
		attachment_type = GL_RGB;
	else if (depth && !stencil)
		attachment_type = GL_DEPTH_COMPONENT;
	else if (!depth && stencil)
		attachment_type = GL_STENCIL_INDEX;

	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	if (!depth && !stencil)
		glTexImage2D(GL_TEXTURE_2D, 0, attachment_type, this->Width, this->Height, 0, attachment_type, GL_UNSIGNED_BYTE, NULL);
	else // Using both a stencil and depth test, needs special format arguments
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, this->Width, this->Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

float Game::GetDeterminant(glm::vec3 k)
{
	float val = (k.x * k.x) + (k.y * k.y) + (k.z * k.z);
	val = pow(val, 0.5f);
	return val;
}

glm::vec3 Game::MultiplyVector(glm::vec3 a, float k)
{
	return glm::vec3(a.x*k,a.y*k,a.z*k);
}

void Game::TestWithFlock()
{
	if (DistanceBetweenVectors(Camera::instance()->GetPosition(), centreOfFlock2) < 5000.0f)
	{
		flock2->setSeperate(true);
		flock2->setSeperationDelay(15.0f);
		flock2->SetPatternMovement(false);
		flock2->SetPatternNumber(0);
	}

	else
	{
		flock2->setSeperate(false);
		flock2->setSeperationDelay(0.0f);
		flock2->SetPatternMovement(true);
		flock2->SetPatternNumber(2);
		flock2->SetSeperatorOne();
	}
}

float Game::DistanceBetweenVectors(glm::vec3 a, glm::vec3 b)
{
	float x = a.x - b.x;
	float y = a.y - b.y;
	float z = a.z - b.z;

	float distance = (x*x) + (y*y) + (z*z);
	return distance;
}

void Game::CleanUp()
{
	delete sphere, planet, pinkPlanet;
	for (int i = 0; i < pointLightContainers.size(); i++)
		delete pointLightContainers[i];

	delete directionalLight;

	for (int i = 0; i < pointLights.size(); i++)
		delete pointLights[i];

	delete particlesystem1;
	delete particlesystem2;

	delete coral1;

	for (int i = 0; i < corals.size(); i++)
	{
		delete corals[i];
	}

	for (int i = 0; i < coralParticles.size(); i++)
	{
		delete coralParticles[i];
	}

	for (int i = 0; i < surfaceEmitter.size(); i++)
	{
		delete surfaceEmitter[i];
	}

	for (int i = 0; i < bgObjs.size(); i++)
	{
		delete bgObjs[i];
	}

	for (int i = 0; i < satellites.size(); i++)
	{
		delete satellites[i];
	}

	delete flock1;
	delete flock2;
	delete flock3;

}


 ovrGraphicsLuid Game::GetDefaultAdapterLuid()
{
	ovrGraphicsLuid luid = ovrGraphicsLuid();

#if defined(_WIN32)
	IDXGIFactory* factory = nullptr;

	if (SUCCEEDED(CreateDXGIFactory(IID_PPV_ARGS(&factory))))
	{
		IDXGIAdapter* adapter = nullptr;

		if (SUCCEEDED(factory->EnumAdapters(0, &adapter)))
		{
			DXGI_ADAPTER_DESC desc;

			adapter->GetDesc(&desc);
			memcpy(&luid, &desc.AdapterLuid, sizeof(luid));
			adapter->Release();
		}

		factory->Release();
	}
#endif

	return luid;
}

int Game::Compare(const ovrGraphicsLuid& lhs, const ovrGraphicsLuid& rhs)
{
	return memcmp(&lhs, &rhs, sizeof(ovrGraphicsLuid));
}



// return true to retry later (e.g. after display lost)
bool Game::RenderOculus()
{
	TextureBuffer * eyeRenderTexture[2] = { nullptr, nullptr };
	DepthBuffer   * eyeDepthBuffer[2] = { nullptr, nullptr };
	ovrMirrorTexture mirrorTexture = nullptr;
	GLuint          mirrorFBO = 0;


	long long frameIndex = 0;

	ovrSession session;
	ovrGraphicsLuid luid;
	ovrResult result = ovr_Create(&session, &luid);
	if (!OVR_SUCCESS(result))
		return retryCreate;

	if (Compare(luid, GetDefaultAdapterLuid())) // If luid that the Rift is on is not the default adapter LUID...
	{
		VALIDATE(false, "OpenGL supports only the default graphics adapter.");
	}

	ovrHmdDesc hmdDesc = ovr_GetHmdDesc(session);

	// Setup Window and Graphics
	// Note: the mirror window can be any size, for this sample we use 1/2 the HMD resolution
	ovrSizei windowSize = { hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2 };

	if (glfwWindowShouldClose(window))
		goto Done;

	// Make eye render buffers
	for (int eye = 0; eye < 2; ++eye)
	{
		ovrSizei idealTextureSize = ovr_GetFovTextureSize(session, ovrEyeType(eye), hmdDesc.DefaultEyeFov[eye], 1);
		eyeRenderTexture[eye] = new TextureBuffer(session, true, true, idealTextureSize, 1, NULL, 1);
		eyeDepthBuffer[eye] = new DepthBuffer(eyeRenderTexture[eye]->GetSize(), 0);

		if (!eyeRenderTexture[eye]->TextureChain)
		{
			if (retryCreate) goto Done;
			VALIDATE(false, "Failed to create texture.");
		}
	}

	ovrMirrorTextureDesc desc;
	memset(&desc, 0, sizeof(desc));
	desc.Width = windowSize.w;
	desc.Height = windowSize.h;
	desc.Format = OVR_FORMAT_R8G8B8A8_UNORM_SRGB;

	// Create mirror texture and an FBO used to copy mirror texture to back buffer
	result = ovr_CreateMirrorTextureGL(session, &desc, &mirrorTexture);
	if (!OVR_SUCCESS(result))
	{
		if (retryCreate) goto Done;
		VALIDATE(false, "Failed to create mirror texture.");
	}

	// Configure the mirror read buffer
	GLuint texId;
	ovr_GetMirrorTextureBufferGL(session, mirrorTexture, &texId);

	glGenFramebuffers(1, &mirrorFBO);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
	glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texId, 0);
	glFramebufferRenderbuffer(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

	// Turn off vsync to let the compositor do its magic
	//wglSwapIntervalEXT(0);

	//manju_change
	// Make scene - can simplify further if needed
	//manju_test
	//roomScene = new Scene(false);
	Init();

	// FloorLevel will give tracking poses where the floor height is 0
	ovr_SetTrackingOriginType(session, ovrTrackingOrigin_FloorLevel);

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		ovrSessionStatus sessionStatus;
		ovr_GetSessionStatus(session, &sessionStatus);
		if (sessionStatus.ShouldQuit)
		{
			// Because the application is requested to quit, should not request retry
			retryCreate = false;
			break;
		}
		if (sessionStatus.ShouldRecenter)
			ovr_RecenterTrackingOrigin(session);

		if (sessionStatus.IsVisible)
		{
			// Keyboard inputs to adjust player orientation
			static float Yaw(3.141592f);
			//if (Platform.Key[VK_LEFT])  Yaw += 0.02f;
			//if (Platform.Key[VK_RIGHT]) Yaw -= 0.02f;

			// Keyboard inputs to adjust player position
			static Vector3f Pos2(0.0f, 0.0f, -5.0f);
			//if (Platform.Key['W'] || Platform.Key[VK_UP])     Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(0, 0, -0.05f));
			//if (Platform.Key['S'] || Platform.Key[VK_DOWN])   Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(0, 0, +0.05f));
			//if (Platform.Key['D'])                            Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(+0.05f, 0, 0));
			//if (Platform.Key['A'])                            Pos2 += Matrix4f::RotationY(Yaw).Transform(Vector3f(-0.05f, 0, 0));

			// Animate the cube
			//manju_change
			//manju_test
			//static float cubeClock = 0;
			//roomScene->Models[0]->Pos = Vector3f(9 * (float)sin(cubeClock), 3, 9 * (float)cos(cubeClock += 0.015f));

			// Call ovr_GetRenderDesc each frame to get the ovrEyeRenderDesc, as the returned values (e.g. HmdToEyeOffset) may change at runtime.
			ovrEyeRenderDesc eyeRenderDesc[2];
			eyeRenderDesc[0] = ovr_GetRenderDesc(session, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
			eyeRenderDesc[1] = ovr_GetRenderDesc(session, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

			// Get eye poses, feeding in correct IPD offset
			ovrPosef                  EyeRenderPose[2];
			ovrVector3f               HmdToEyeOffset[2] = { eyeRenderDesc[0].HmdToEyeOffset,
				eyeRenderDesc[1].HmdToEyeOffset };

			double sensorSampleTime;    // sensorSampleTime is fed into the layer later
			ovr_GetEyePoses(session, frameIndex, ovrTrue, HmdToEyeOffset, EyeRenderPose, &sensorSampleTime);

			// Render Scene to Eye Buffers
			for (int eye = 0; eye < 2; ++eye)
			{
				// Switch to eye render target
				eyeRenderTexture[eye]->SetAndClearRenderSurface(eyeDepthBuffer[eye]);

				// Get view and projection matrices
				Matrix4f rollPitchYaw = Matrix4f::RotationY(Yaw);
				Matrix4f finalRollPitchYaw = rollPitchYaw * Matrix4f(EyeRenderPose[eye].Orientation);
				Vector3f finalUp = finalRollPitchYaw.Transform(Vector3f(0, 1, 0));
				Vector3f finalForward = finalRollPitchYaw.Transform(Vector3f(0, 0, -1));
				Vector3f shiftedEyePos = Pos2 + rollPitchYaw.Transform(EyeRenderPose[eye].Position);

				//manju_note
				Matrix4f view = Matrix4f::LookAtRH(shiftedEyePos, shiftedEyePos + finalForward, finalUp);
				Matrix4f proj = ovrMatrix4f_Projection(hmdDesc.DefaultEyeFov[eye], 0.2f, 1000.0f, ovrProjection_None);

				//manju_change
				// Render world
				//manju_test
				//roomScene->Render(view, proj);
				//for (int i = 0; i < game.AllModels.size(); ++i)
				calcFPS(1.0, windowTitle);

				// Set frame time
				GLfloat currentFrame = glfwGetTime();
				deltaTime = currentFrame - lastFrame;
				lastFrame = currentFrame;

				// Check and call events
				glfwPollEvents();

				// Manage user input
				ProcessInput(deltaTime);

				// Update Game state
				Update(deltaTime);

				// Clear the colorbuffer
				glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



				//glm::mat4 viewVR; glm::mat4 projectionVR;
				Render();

				// Avoids an error when calling SetAndClearRenderSurface during next iteration.
				// Without this, during the next while loop iteration SetAndClearRenderSurface
				// would bind a framebuffer with an invalid COLOR_ATTACHMENT0 because the texture ID
				// associated with COLOR_ATTACHMENT0 had been unlocked by calling wglDXUnlockObjectsNV.
				eyeRenderTexture[eye]->UnsetRenderSurface();

				// Commit changes to the textures so they get picked up frame
				eyeRenderTexture[eye]->Commit();
			}

			// Do distortion rendering, Present and flush/sync

			ovrLayerEyeFov ld;
			ld.Header.Type = ovrLayerType_EyeFov;
			ld.Header.Flags = ovrLayerFlag_TextureOriginAtBottomLeft;   // Because OpenGL.

			for (int eye = 0; eye < 2; ++eye)
			{
				ld.ColorTexture[eye] = eyeRenderTexture[eye]->TextureChain;
				ld.Viewport[eye] = Recti(eyeRenderTexture[eye]->GetSize());
				ld.Fov[eye] = hmdDesc.DefaultEyeFov[eye];
				ld.RenderPose[eye] = EyeRenderPose[eye];
				ld.SensorSampleTime = sensorSampleTime;
			}

			ovrLayerHeader* layers = &ld.Header;
			result = ovr_SubmitFrame(session, frameIndex, nullptr, &layers, 1);
			// exit the rendering loop if submit returns an error, will retry on ovrError_DisplayLost
			if (!OVR_SUCCESS(result))
				goto Done;

			frameIndex++;
		}

		// Blit mirror texture to back buffer
		glBindFramebuffer(GL_READ_FRAMEBUFFER, mirrorFBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
		GLint w = windowSize.w;
		GLint h = windowSize.h;
		glBlitFramebuffer(0, h, w, 0,
			0, 0, w, h,
			GL_COLOR_BUFFER_BIT, GL_NEAREST);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

		glfwSwapBuffers(window);
	}

Done:
	//manju_change
	//manju_test
	//delete roomScene;
	Running = false;
	if (mirrorFBO) glDeleteFramebuffers(1, &mirrorFBO);
	if (mirrorTexture) ovr_DestroyMirrorTexture(session, mirrorTexture);
	for (int eye = 0; eye < 2; ++eye)
	{
		delete eyeRenderTexture[eye];
		delete eyeDepthBuffer[eye];
	}
	//Platform.ReleaseDevice();
	ovr_Destroy(session);

	// Retry on ovrError_DisplayLost
	return retryCreate || (result == ovrError_DisplayLost);
}

void Game::RunVR()
{
	while (!glfwWindowShouldClose(window))
	{
		if(retryCreate)
		RenderOculus();

	}
}


double Game::calcFPS(double theTimeInterval, string theWindowTitle)
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