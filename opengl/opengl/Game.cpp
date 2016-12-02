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
	ResourceManager::LoadShader("Shaders/vertexShader_default.vs", "Shaders/fragmentShader_default.frag", nullptr, "default");
	spaceShip.InitPath("Models/SpaceCraft/Wraith Raider Starship.obj");
	spaceShip.SetPosition(glm::vec3(0.0f, -1.75f, 10.0f));
	spaceShip.SetScale(glm::vec3(0.01f, 0.01f, 0.01f));

	ResourceManager::LoadShader("Shaders/vertexShader_Skybox.vs", "Shaders/fragmentShader_Skybox.frag", nullptr, "skybox");
	skybox.setupMesh();

	glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f);
	glm::vec3 ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	glm::vec3 diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 specular = glm::vec3(1.0f, 1.0f, 1.0f);
	directionalLight.initializeDirectionalLightParameters(direction, ambient, diffuse, specular);

	masterBullet = new Bullet();
	masterBullet->InitPath("Models/Bullet/Bullet.obj");
	GenerateEnvironment();
	
}

void Game::Update(GLfloat dt)
{

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
		
		if (this->Keys[GLFW_KEY_SPACE])
		{
			currentBulletFired = glfwGetTime();
			float shootInterval = currentBulletFired - previousBulletFired;
			if (shootInterval > 0.1f)
			{
				Shoot();
			}

			previousBulletFired = currentBulletFired;
		}
	}
}

void Game::Render(GLfloat dt)
{	
	glm::mat4 view = Camera::instance()->GetViewMatrix();
	glm::mat4 projection = glm::perspective(Camera::instance()->Zoom, static_cast<GLfloat>(this->Width) / static_cast<GLfloat>(this->Height), 0.1f, 100.0f);

	Shader shader = ResourceManager::GetShader("default");
	shader.Use();
	shader.SetMatrix4("view", view);
	shader.SetMatrix4("projection", projection);
	directionalLight.setUpDirectionalLight(&ResourceManager::GetShader("default"), Camera::instance());
	glm::quat myQuat;
	glm::quat key_quat = glm::quat(glm::vec3(0.0f, (GLfloat)glfwGetTime() * glm::radians(20.0f), 0.0f));
	myQuat = key_quat * myQuat;
	myQuat = glm::normalize(myQuat);
	spaceShip.SetRotation(myQuat);
	spaceShip.Draw(&shader);

	for (int i = 0; i < physicsObjects.size(); i++)
	{
		physicsObjects[i]->ProcessAI(Camera::instance()->Position);
		physicsObjects[i]->UpdateCollider(dt);
		physicsObjects[i]->Draw(&ResourceManager::GetShader("default"));
		for (int j = 0; j < physicsObjects[i]->enemyBullets.size(); j++)
		{
			physicsObjects[i]->enemyBullets[j]->UpdateCollider(dt);
			physicsObjects[i]->enemyBullets[j]->Draw(&ResourceManager::GetShader("default"));
		}
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->UpdateCollider(dt);
		bullets[i]->Draw(&ResourceManager::GetShader("default"));
	}
	
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
	Shader skyboxShader = ResourceManager::GetShader("skybox");
	skyboxShader.Use();
	view = glm::mat4(glm::mat3(Camera::instance()->GetViewMatrix()));	// Remove any translation component of the view matrix
	skyboxShader.SetMatrix4("view", view);
	skyboxShader.SetMatrix4("projection", projection);
	skybox.Draw(&skyboxShader);
	glDepthFunc(GL_LESS); // Set depth function back to default
	
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