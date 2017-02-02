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
	delete masterBullet;
}

void Game::Init()
{

	planet = new Model();
	pinkPlanet = new Model();
	sphere = new Model();
	pointLightContainer = new Model();
	



	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				BGMovingObjects *obj = new BGMovingObjects();
				obj->InitPath("Models/Bullet/Bullet.obj");
				obj->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
				obj->SetPosition(glm::vec3(-i*10 - 30,-j*10,-k*10 - 30));
				obj->SetVelocity(glm::vec3(0.0f, 0.0f, -0.1f));
				movingObjs1.push_back(obj);
			}
		}
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				BGMovingObjects *obj = new BGMovingObjects();
				obj->InitPath("Models/Bullet/Bullet.obj");
				obj->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
				obj->SetPosition(glm::vec3(i * 10 + 30, j * 10, k * 10 + 30));
				obj->SetVelocity(glm::vec3(0.0f, 0.0f, -0.1f));
				movingObjs2.push_back(obj);
			}
		}
	}

	vector<glm::vec3> coralPositions;
	//coral positions
	glm::vec3 coralPosition1 = glm::vec3(1, 12, 23.75); coralPositions.push_back(coralPosition1);
	glm::vec3 coralPosition2 = glm::vec3(1, 15.5, 20.25); coralPositions.push_back(coralPosition2); 
	glm::vec3 coralPosition3 = glm::vec3(5.5, 15.25, 19.25); coralPositions.push_back(coralPosition3);
	glm::vec3 coralPosition4 = glm::vec3(7, 18.5, 15.25); coralPositions.push_back(coralPosition4);
	glm::vec3 coralPosition5 = glm::vec3(-1.75, 21, 15.25); coralPositions.push_back(coralPosition5); 
	glm::vec3 coralPosition6 = glm::vec3(-0.75, 22.25, 12.25); coralPositions.push_back(coralPosition6);
	glm::vec3 coralPosition7 = glm::vec3(-1.75, 23.75, 9); coralPositions.push_back(coralPosition7);
	glm::vec3 coralPosition8 = glm::vec3(-4.5, 23.25, 7.5); coralPositions.push_back(coralPosition8);
	glm::vec3 coralPosition9 = glm::vec3(-2.25, 24, 4.5); coralPositions.push_back(coralPosition9);
	glm::vec3 coralPosition10 = glm::vec3(2, 24.25, 2); coralPositions.push_back(coralPosition10);

	

	// Load Default Shader
	ResourceManager::LoadShader("Shaders/vertexShader_default.vs", "Shaders/fragmentShader_default.frag", nullptr, "default");

	planet->InitPath("Models/Pink Planet/untitled1.obj");
	planet->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	planet->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	pinkPlanet->InitPath("Models/Pink Planet/untitled1.obj");
	pinkPlanet->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	pinkPlanet->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));


	bgObject.InitPath("Models/Bullet/Bullet.obj");
	bgObject.SetPosition(glm::vec3(10.0f, 0.0f, 25.0f));
	bgObject.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
	
	bgObject2.InitPath("Models/Bullet/Bullet.obj");
	bgObject2.SetPosition(glm::vec3(20.0f, 0.0f, 25.0f));
	bgObject2.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));

	bgObject3.InitPath("Models/Bullet/Bullet.obj");
	bgObject3.SetPosition(glm::vec3(-10.0f, 0.0f, 25.0f));
	bgObject3.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));

	bgObject4.InitPath("Models/Bullet/Bullet.obj");
	bgObject4.SetPosition(glm::vec3(-20.0f, 0.0f, 25.0f));
	bgObject4.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));

	

	ResourceManager::LoadShader("Shaders/vertexShader_LightContainer.vs", "Shaders/fragmentShader_LightContainer.frag", nullptr, "coralShader");
	
	Coral *masterCoral = new Coral();
	masterCoral->InitPath("Models/Corals/coral0.obj");
	masterCoral->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));


	for (int i = 0;i < coralPositions.size();i++)
	{
		Coral * coral = new Coral();
		*coral = *masterCoral;
		coral->SetPosition(coralPositions[i]);
		corals.push_back(coral);
	}

	//initializing corals
	


	//spaceShip.InitPath("Models/SpaceCraft/Wraith Raider Starship.obj");
	//spaceShip.SetPosition(glm::vec3(0.0f, -1.75f, 2.0f));
	//spaceShip.SetScale(glm::vec3(0.01f, 0.01f, 0.01f));

	// Load Skybox Shader
	//ResourceManager::LoadShader("Shaders/vertexShader_Skybox.vs", "Shaders/fragmentShader_Skybox.frag", nullptr, "skybox");
	//skybox.setupMesh();

	// Setup Directional Light
	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight.initializeDirectionalLightParameters(direction, ambient, diffuse, specular);

	// Initialize Point Light (Initialize as many point lights as requires and push the lights into the pointLights vector)
	// Cuurently the max number of point lights is set as 10 in fragment shader. For more lights change the value in fragment shader
	Lights pointLight;
	pointLights.push_back(pointLight);

	//masterBullet = new Bullet();
	//masterBullet->InitPath("Models/Bullet/Bullet.obj");
	//GenerateEnvironment();

	// Point Light Container Shader
	/*ResourceManager::LoadShader("Shaders/vertexShader_LightContainer.vs", "Shaders/fragmentShader_LightContainer.frag", nullptr, "lightContainerShader");
	pointLightContainer.InitPath("Models/Bullet/Bullet.obj");
	pointLightContainer.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	pointLightContainer.SetScale(glm::vec3(10.0f, 10.0f, 10.0f));*/

	ResourceManager::LoadShader("Shaders/vertexShader_Skysphere.vs", "Shaders/fragmentShader_Skysphere.frag", nullptr, "skySphere");
	sphere->InitPath("Models/Sphere/sphere.obj");
	sphere->SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	sphere->SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

	//loading resources
	ResourceManager::LoadShader("Shaders/particle.vs", "Shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadTexture("Textures/fireParticle.png", GL_TRUE, "particle");
	particlesystem1 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	particlesystem1->age = 10.0f;
	particlesystem1->initialPosition = glm::vec3(0.5, 0.5, 60.0);
	//particlesystem1->acceleration = glm::vec3(0.0, 1.0, 0.0);
	particlesystem1->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	particlesystem1->startVelocityMin = 0.1f;
	particlesystem1->startVelocityRange = 0.1f;
	particlesystem1->scale = 0.1f;



	particlesystem2 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	particlesystem2->age = 10.0f;
	particlesystem2->initialPosition = glm::vec3(-3, 3, 50);
	//particlesystem2->acceleration = glm::vec2(0.0, 0.0);
	particlesystem2->color = glm::vec4(0.1f, 0.8f, 0.4f, 1.0f);
	particlesystem2->startVelocityMin = 0.1f;
	particlesystem2->startVelocityRange = 0.1f;
	particlesystem2->scale = 0.1f;

	//coral particle system

	ParticleSystem *masterCoralParticle = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	masterCoralParticle->age = 10.0f;
	masterCoralParticle->acceleration = glm::vec3(0.0, 1.0, 0.0);
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



}

void Game::Update(GLfloat dt)
{
	/*for (int i = 0; i < physicsObjects.size(); i++)
	{
		physicsObjects[i]->ProcessAI(Camera::instance()->Position);
		physicsObjects[i]->UpdateCollider(dt);
		for (int j = 0; j < physicsObjects[i]->enemyBullets.size(); j++)
		{
			physicsObjects[i]->enemyBullets[j]->UpdateCollider(dt);
		}
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->UpdateCollider(dt);
	}*/
	particlesystem1->Update(dt, 2);
	particlesystem2->Update(dt, 2);
	//coral particle update

	for (int i = 0;i < coralParticles.size();i++)
	{
		coralParticles[i]->Update(dt, 2);
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

		if (this->Keys[GLFW_KEY_I])
		{
			bgY += 0.25;
		}
			//bgObject.SetPosition(glm::vec3(bgObject.GetPosition().x,))

		/*if (this->Keys[GLFW_KEY_I])
		{
			coralPosition.y += .25;
		}
		if (this->Keys[GLFW_KEY_K])
		{
			coralPosition.y -= .25;
		}
		if (this->Keys[GLFW_KEY_J])
		{
			coralPosition.z += .25;
		}
		if (this->Keys[GLFW_KEY_L])
		{
			coralPosition.z -= .25;
		}
		if (this->Keys[GLFW_KEY_U])
		{
			coralPosition.x += .25;
		}
		if (this->Keys[GLFW_KEY_O])
		{
			coralPosition.x -= .25;
		}*/
		
		if (this->Keys[GLFW_KEY_SPACE])
		{
			/*currentBulletFired = glfwGetTime();
			float shootInterval = currentBulletFired - previousBulletFired;
			if (shootInterval > 0.1f)
			{
				Shoot();
			}

			previousBulletFired = currentBulletFired;*/
			//std::cout << coralPosition.x << coralPosition.y << coralPosition.z << std::endl;
			//cout << "Bezier " << coralPosition.x << " " << coralPosition.y << " " << coralPosition.z << endl;
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
	glm::mat4 view;
	glm::mat4 projection;
	if (isVR)
	{
		 view = Camera::instance()->GetViewMatrix();
		 projection = glm::perspective(Camera::instance()->Zoom, static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 100.0f);
	}

	else
	{
		 view = Camera::instance()->GetViewMatrix();
		 projection = glm::perspective(Camera::instance()->Zoom, static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 100.0f);
	}
	Shader shader = ResourceManager::GetShader("default");
	shader.Use();
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("projection", projection);
	
	// Setup Directional Light
	directionalLight.setUpDirectionalLight(&ResourceManager::GetShader("default"), Camera::instance());

	// Setup Point Light. Properties of Point Light can be changed over time if required.
	// Get the Point Light whose values need to be changed using the vector pointLights and change the properties as required
	glm::vec3 position = glm::vec3(25.0f * sin(glfwGetTime() * 1.0f), -30.0f, 25.0f * cos(glfwGetTime() * 1.0f));
	//glm::vec3 ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	glm::vec3 ambient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	//pointLightContainer.SetPosition(glm::vec3(25.0f * sin(glfwGetTime() * 1.0f), -30.0f, 25.0f * cos(glfwGetTime() * 1.0f)));
	for (int i = 0; i < pointLights.size(); i++)
	{
		// Note that the 4th parameter is the distance the point light should affect (set the distance from the pre initialized pointLightDistance array)
		pointLights[i].setPointLightParameters(position, ambient, diffuse, specular, pointLightDistance[4]);
		pointLights[i].setUpPointLight(&ResourceManager::GetShader("default"), Camera::instance());
	}
	ResourceManager::GetShader("default").SetInteger("NO_OF_POINT_LIGHTS", pointLights.size());
	
	//glm::quat myQuat;
	//glm::quat key_quat = glm::quat(glm::vec3(0.0f, (GLfloat)glfwGetTime() * glm::radians(20.0f), 0.0f));
	//myQuat = key_quat * myQuat;
	//myQuat = glm::normalize(myQuat);
	//spaceShip.SetRotation(myQuat);
	//spaceShip.Draw(&shader);

	if(!changePlanet)
		planet->Draw(&shader);
	else {
		pinkPlanet->Draw(&shader);
	}
	bgObject.Update(&shader, Camera::instance()->GetPosition());
	bgObject2.Update(&shader, Camera::instance()->GetPosition());
	bgObject3.Update(&shader, Camera::instance()->GetPosition());
	bgObject4.Update(&shader, Camera::instance()->GetPosition());

	for (int i = 0; i < 27; i++)
	{
		movingObjs1[i]->Update(&shader);
		movingObjs2[i]->Update(&shader);
	}

	//movingObj1->Update(&shader);

	Shader coralShader = ResourceManager::GetShader("coralShader");
	coralShader.Use();
	coralShader.SetMatrix4("view", view);
	coralShader.SetMatrix4("projection", projection);
	
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

	/*coral1.Draw(&shader);
	coral2.Draw(&shader);
	coral3.Draw(&shader);
	coral4.Draw(&shader);
	coral5.Draw(&shader);
	coral6.Draw(&shader);
	coral7.Draw(&shader);
	coral8.Draw(&shader);
	coral9.Draw(&shader);
	coral10.Draw(&shader);*/

	
	/*for (int i = 0; i < physicsObjects.size(); i++)
	{
		physicsObjects[i]->Draw(&ResourceManager::GetShader("default"));
		for (int j = 0; j < physicsObjects[i]->enemyBullets.size(); j++)
		{
			physicsObjects[i]->enemyBullets[j]->Draw(&ResourceManager::GetShader("default"));
		}
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Draw(&ResourceManager::GetShader("default"));
	}*/

	// Also draw the point light object, again binding the appropriate shader
	///*Shader lampShader = ResourceManager::GetShader("lightContainerShader");
	//lampShader.Use();
	//lampShader.SetMatrix4("view", view);
	//lampShader.SetMatrix4("projection", projection);
	//pointLightContainer.Draw(&lampShader);*/

	glDepthFunc(GL_LEQUAL);
	Shader skySphereShader = ResourceManager::GetShader("skySphere");
	skySphereShader.Use();
	view = glm::mat4(glm::mat3(Camera::instance()->GetViewMatrix()));	// Remove any translation component of the view matrix
	skySphereShader.SetMatrix4("view", view);
	skySphereShader.SetMatrix4("projection", projection);
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
		changePlanet = true;
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
	view = Camera::instance()->GetViewMatrix();
	particleShader.SetMatrix4("view", view);
	particleShader.SetMatrix4("projection", projection);
	glm::mat4 model;
	//model = glm::translate(model, particle.Position);
	model[0][0] = view[0][0];
	model[0][1] = view[1][0];
	model[0][2] = view[2][0];
	model[1][0] = view[0][1];
	model[1][1] = view[1][1];
	model[1][2] = view[2][1];
	model[2][0] = view[0][2];
	model[2][1] = view[1][2];
	model[2][2] = view[2][2];
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



	particlesystem1->Draw();
	particlesystem2->Draw();
	//coral particle draw

	for (int i = 0;i < coralParticles.size();i++)
	{
		coralParticles[i]->Draw();
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
	
	DetectCollisions();
}

// Other methods specific to our game
//typeOfObject : 2 means its enemy, 3 means its an obstacle
bool Game::IsPositionValid(std::tuple<int, int, int> positionTuple, int typeOfObject)
{
	bool flag = false;
	int minDistanceBetweenEnemies = 25;
	int minDistanceBetweenObstacles = 25;
	int minDistanceBetweenOE = 15;

	if (mapPositions[positionTuple] == NULL)
	{
		flag = true;
		for (auto it = mapPositions.begin(); it != mapPositions.end(); it++)
		{
			int x1 = get<0>(it->first); int x2 = get<0>(positionTuple);
			int y1 = get<1>(it->first); int y2 = get<1>(positionTuple);
			int z1 = get<2>(it->first); int z2 = get<2>(positionTuple);

			int d1 = pow(x1 - x2, 2);
			int d2 = pow(y1 - y2, 2);
			int d3 = pow(z1 - z2, 2);
			int value = it->second;
			switch (typeOfObject)
			{
			case 2:
				if (value == 2)
				{
					if ((d1 + d2 + d3) < minDistanceBetweenEnemies && (d1 + d2 + d3)>0)
					{
						mapPositions.erase(positionTuple);
						flag = false;
						return flag;
					}
				}

				else if (value == 3)
				{
					if ((d1 + d2 + d3) < minDistanceBetweenOE && (d1 + d2 + d3)>0)
					{
						mapPositions.erase(positionTuple);
						flag = false;
						return flag;
					}
				}
				break;

			case 3:
				if (value == 2)
				{
					if ((d1 + d2 + d3) < minDistanceBetweenOE && (d1 + d2 + d3)>0)
					{
						mapPositions.erase(positionTuple);
						flag = false;
						return flag;
					}
				}

				else if (value == 3)
				{
					if ((d1 + d2 + d3) < minDistanceBetweenObstacles && (d1 + d2 + d3)>0)
					{
						mapPositions.erase(positionTuple);
						flag = false;
						return flag;
					}
				}
				break;

			}
		}
		return flag;
	}
	mapPositions.erase(positionTuple);
	return flag;
}

void Game::DetectCollisions()
{
	//Check for collision between bullets by player and enemy objects
	for (int i = 0; i < physicsObjects.size(); )
	{
		bool delObj = false;
		for (int j = 0; j < bullets.size(); )
		{
			glm::vec3 distance = physicsObjects[i]->GetPosition() - bullets[j]->GetPosition();
			float sumOfRadii = physicsObjects[i]->GetCollider()->GetRadius() + bullets[j]->GetCollider()->GetRadius();
			float length = (distance.x * distance.x) + (distance.y * distance.y) + (distance.z * distance.z);

			//Destroy the bullet
			if (length < (sumOfRadii * sumOfRadii))
			{
				//Clean way to destroy the bullet
				Bullet *bullet = bullets[j];
				bullets.erase(bullets.begin() + j);
				delete bullet;
				physicsObjects[i]->ReduceHealth(10);
			}

			else if (bullets[j]->GetPosition().x > 100 || bullets[j]->GetPosition().x < -100 ||
				bullets[j]->GetPosition().y > 100 || bullets[j]->GetPosition().y < -100 ||
				bullets[j]->GetPosition().z > 100 || bullets[j]->GetPosition().z < -100)
			{
				Bullet *bullet = bullets[j];
				bullets.erase(bullets.begin() + j);
				delete bullet;
				physicsObjects[i]->ReduceHealth(10);
			}

			else
			{
				++j;
			}
		}

		//Destroy the enemy
		if (physicsObjects[i]->GetHealth() <= 0)
		{
			delObj = true;
			NormalEnemy *enemy = physicsObjects[i];
			physicsObjects.erase(physicsObjects.begin() + i);
			delete enemy;
		}
		else
		{
			++i;
		}
	}
}

void Game::Shoot()
{
	Bullet *enemy = new Bullet();
	*enemy = *masterBullet;
	glm::vec3 shootPosition = glm::vec3(Camera::instance()->GetPosition().x, Camera::instance()->GetPosition().y - 1.0f, Camera::instance()->GetPosition().z);
	enemy->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
	enemy->AddSphereCollider(2.0f, enemy->GetPosition());
	glm::vec3 bulletDirection = glm::normalize(Camera::instance()->Front);
	float bulletSpeed = 4.0f;
	bulletDirection = glm::vec3(bulletDirection * bulletSpeed);
	enemy->SetValues(shootPosition, bulletDirection);
	bullets.push_back(enemy);
}

void Game::GenerateEnvironment()
{
	/*ifstream inputFile;
	inputFile.open("EnvironmentCoordinates/TrainedValues.txt");
	vector< vector<int> > coordinates;

	NormalEnemy *masterEnemy = new NormalEnemy();
	masterEnemy->InitPath("Models/SpaceCraft/Wraith Raider Starship.obj");
	masterEnemy->SetType(1);

	NormalEnemy *masterAsteroid = new NormalEnemy();
	masterAsteroid->InitPath("Models/Asteroid/Asteroid.obj");
	masterAsteroid->SetType(0);

	for (int i = 0; i < 23; i++)
	{
	vector<int> temp;
	char tempString = NULL;
	for (int j = 0; j < 40; j++)
	{
	inputFile >> tempString;
	int input = tempString - '0';
	if (input == 1)
	{
	NormalEnemy *enemy = new NormalEnemy();
	*enemy = *masterEnemy;
	enemy->SetValues(glm::vec3(i*5, (rand() % 5 - 5), j*-10), glm::vec3(0, 0, 0));
	enemy->AddSphereCollider(2.0f, enemy->GetPosition());
	enemy->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
	enemy->SetHealth(100);
	physicsObjects.push_back(enemy);
	}

	if (input == 2)
	{
	NormalEnemy *enemy = new NormalEnemy();
	*enemy = *masterAsteroid;
	enemy->SetValues(glm::vec3(i * 5, (rand() % 5 - 5), j * -10), glm::vec3(0, 0, 0));
	enemy->AddSphereCollider(2.0f, enemy->GetPosition());
	enemy->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
	enemy->SetHealth(100);
	physicsObjects.push_back(enemy);
	}

	temp.push_back(input);
	tempString = NULL;
	}
	coordinates.push_back(temp);
	temp.clear();
	}
	int k = 0;*/

	NormalEnemy *masterEnemy = new NormalEnemy();
	masterEnemy->InitPath("Models/SpaceCraft/Wraith Raider Starship.obj");
	masterEnemy->SetType(1);
	masterEnemy->SetValues(glm::vec3(0 * 5, (rand() % 5 - 5), 1 * -10), glm::vec3(0, 0, 0));
	masterEnemy->AddSphereCollider(2.0f, masterEnemy->GetPosition());
	masterEnemy->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
	masterEnemy->SetHealth(100);
	physicsObjects.push_back(masterEnemy);


	/*bool checkPositionValidity = true;
	int numberOfObjects = 20;
	NormalEnemy *masterEnemy = new NormalEnemy();
	masterEnemy->InitPath("Models/SpaceCraft/Wraith Raider Starship.obj");
	for (int i = 0; i < numberOfObjects; i++)
	{
	NormalEnemy *enemy = new NormalEnemy();
	*enemy = *masterEnemy;
	while (checkPositionValidity == true)
	{
	enemy->SetValues(glm::vec3(rand() % 10, (rand() % 5 - 5), (rand() % 50) * -1),glm::vec3(0,0,0));
	auto positionTuple = make_tuple(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
	if (IsPositionValid(positionTuple,3) == true)
	{
	checkPositionValidity = false;
	}
	}
	checkPositionValidity = true;
	auto positionTuple = make_tuple(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
	mapPositions[positionTuple] = 2;
	enemy->AddSphereCollider(2.0f, enemy->GetPosition());
	enemy->SetScale(glm::vec3(0.01f, 0.01f, 0.01f));
	enemy->SetHealth(100);
	physicsObjects.push_back(enemy);
	}*/
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