#include "Game.h"
//#include "resource_manager.h"


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
	
	// Load Default Shader
	ResourceManager::LoadShader("Shaders/vertexShader_default.vs", "Shaders/fragmentShader_default.frag", nullptr, "default");

	planet.InitPath("Models/Pink Planet/untitled1.obj");
	planet.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	planet.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	//initializing corals
	coral1.InitPath("Models/Corals/coral0.obj");
	coral1.SetPosition(coralPosition1);
	coral1.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	coral2.InitPath("Models/Corals/coral0.obj");
	coral2.SetPosition(coralPosition2);
	coral2.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	coral3.InitPath("Models/Corals/coral0.obj");
	coral3.SetPosition(coralPosition3);
	coral3.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	coral4.InitPath("Models/Corals/coral0.obj");
	coral4.SetPosition(coralPosition4);
	coral4.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	coral5.InitPath("Models/Corals/coral0.obj");
	coral5.SetPosition(coralPosition5);
	coral5.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	coral6.InitPath("Models/Corals/coral0.obj");
	coral6.SetPosition(coralPosition6);
	coral6.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	coral7.InitPath("Models/Corals/coral0.obj");
	coral7.SetPosition(coralPosition7);
	coral7.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	coral8.InitPath("Models/Corals/coral0.obj");
	coral8.SetPosition(coralPosition8);
	coral8.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	coral9.InitPath("Models/Corals/coral0.obj");
	coral9.SetPosition(coralPosition9);
	coral9.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	coral10.InitPath("Models/Corals/coral0.obj");
	coral10.SetPosition(coralPosition10);
	coral10.SetScale(glm::vec3(0.1f, 0.1f, 0.1f));


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
	sphere.InitPath("Models/Sphere/sphere.obj");
	sphere.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
	sphere.SetScale(glm::vec3(3.0f, 3.0f, 3.0f));

	//loading resources
	ResourceManager::LoadShader("Shaders/particle.vs", "Shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadTexture("Textures/fireParticle.png", GL_TRUE, "particle");
	particlesystem1 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	particlesystem1->age = 1.0f;
	particlesystem1->initialPosition = glm::vec3(0.5, 0.5, 60.0);
	particlesystem1->acceleration = glm::vec3(0.0, 1.0, 0.0);
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
	coralParticle1 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle1->age = 10.0f;
	coralParticle1->initialPosition = coralPosition1;
	coralParticle1->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle1->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle1->startVelocityMin = 0.1f;
	coralParticle1->startVelocityRange = 0.1f;
	coralParticle1->scale = 0.1f;

	coralParticle2 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle2->age = 10.0f;
	coralParticle2->initialPosition = coralPosition2;
	coralParticle2->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle2->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle2->startVelocityMin = 0.1f;
	coralParticle2->startVelocityRange = 0.1f;
	coralParticle2->scale = 0.1f;

	coralParticle3 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle3->age = 10.0f;
	coralParticle3->initialPosition = coralPosition3;
	coralParticle3->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle3->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle3->startVelocityMin = 0.1f;
	coralParticle3->startVelocityRange = 0.1f;
	coralParticle3->scale = 0.1f;

	coralParticle4 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle4->age = 10.0f;
	coralParticle4->initialPosition = coralPosition4;
	coralParticle4->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle4->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle4->startVelocityMin = 0.1f;
	coralParticle4->startVelocityRange = 0.1f;
	coralParticle4->scale = 0.1f;

	coralParticle5 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle5->age = 10.0f;
	coralParticle5->initialPosition = coralPosition5;
	coralParticle5->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle5->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle5->startVelocityMin = 0.1f;
	coralParticle5->startVelocityRange = 0.1f;
	coralParticle5->scale = 0.1f;

	coralParticle6 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle6->age = 10.0f;
	coralParticle6->initialPosition = coralPosition6;
	coralParticle6->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle6->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle6->startVelocityMin = 0.1f;
	coralParticle6->startVelocityRange = 0.1f;
	coralParticle6->scale = 0.1f;

	coralParticle7 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle7->age = 10.0f;
	coralParticle7->initialPosition = coralPosition7;
	coralParticle7->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle7->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle7->startVelocityMin = 0.1f;
	coralParticle7->startVelocityRange = 0.1f;
	coralParticle7->scale = 0.1f;

	coralParticle8 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle8->age = 10.0f;
	coralParticle8->initialPosition = coralPosition8;
	coralParticle8->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle8->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle8->startVelocityMin = 0.1f;
	coralParticle8->startVelocityRange = 0.1f;
	coralParticle8->scale = 0.1f;

	coralParticle9 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle9->age = 10.0f;
	coralParticle9->initialPosition = coralPosition9;
	coralParticle9->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle9->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle9->startVelocityMin = 0.1f;
	coralParticle9->startVelocityRange = 0.1f;
	coralParticle9->scale = 0.1f;

	coralParticle10 = new ParticleSystem(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 1000);
	coralParticle10->age = 10.0f;
	coralParticle10->initialPosition = coralPosition10;
	coralParticle10->acceleration = glm::vec3(0.0, 1.0, 0.0);
	coralParticle10->color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);
	coralParticle10->startVelocityMin = 0.1f;
	coralParticle10->startVelocityRange = 0.1f;
	coralParticle10->scale = 0.1f;

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
	coralParticle1->Update(dt, 2);
	coralParticle2->Update(dt, 2);
	coralParticle3->Update(dt, 2);
	coralParticle4->Update(dt, 2);
	coralParticle5->Update(dt, 2);
	coralParticle6->Update(dt, 2);
	coralParticle7->Update(dt, 2);
	coralParticle8->Update(dt, 2);
	coralParticle9->Update(dt, 2);
	coralParticle10->Update(dt, 2);
	//coral.SetPosition(coralPosition);
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

void Game::Render()
{	
	glm::mat4 view = Camera::instance()->GetViewMatrix();
	glm::mat4 projection = glm::perspective(Camera::instance()->Zoom, static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 100.0f);
	
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

	planet.Draw(&shader);
	coral1.Draw(&shader);
	coral2.Draw(&shader);
	coral3.Draw(&shader);
	coral4.Draw(&shader);
	coral5.Draw(&shader);
	coral6.Draw(&shader);
	coral7.Draw(&shader);
	coral8.Draw(&shader);
	coral9.Draw(&shader);
	coral10.Draw(&shader);

	
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
	Shader lampShader = ResourceManager::GetShader("lightContainerShader");
	lampShader.Use();
	lampShader.SetMatrix4("view", view);
	lampShader.SetMatrix4("projection", projection);
	pointLightContainer.Draw(&lampShader);

	glDepthFunc(GL_LEQUAL);
	Shader skySphereShader = ResourceManager::GetShader("skySphere");
	skySphereShader.Use();
	view = glm::mat4(glm::mat3(Camera::instance()->GetViewMatrix()));	// Remove any translation component of the view matrix
	skySphereShader.SetMatrix4("view", view);
	skySphereShader.SetMatrix4("projection", projection);
	GLfloat timeValue = glfwGetTime();
	skySphereShader.SetFloat("iGlobalTime", timeValue);
	sphere.Draw(&skySphereShader);
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
	coralParticle1->Draw();
	coralParticle2->Draw();
	coralParticle3->Draw();
	coralParticle4->Draw();
	coralParticle5->Draw();
	coralParticle6->Draw();
	coralParticle7->Draw();
	coralParticle8->Draw();
	coralParticle9->Draw();
	coralParticle10->Draw();
	
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