#include "Camera.h"

Camera* Camera::player_instance = 0;
void Camera :: ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
	GLfloat velocity = this->MovementSpeed * deltaTime * speed;
	if (direction == FORWARD)
		this->Position += this->Front * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Front * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;
}

void Camera::SetSpeed(float s)
{
	speed = s;
}

void Camera :: ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

void Camera::MovePositiveZ(GLfloat deltaTime)
{
	/*GLfloat velocity = this->MovementSpeed * deltaTime * 2;
	this->Position -= this->Front * velocity;*/
	this->Position.z += 0.3f;
}

void Camera :: ProcessMouseScroll(GLfloat yoffset)
{
	//if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
	//	this->Zoom -= yoffset;
	//if (this->Zoom <= 1.0f)
	//	this->Zoom = 1.0f;
	//if (this->Zoom >= 45.0f)
	//	this->Zoom = 45.0f;
}

void Camera :: updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 front;
	front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	front.y = sin(glm::radians(this->Pitch));
	front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Front = glm::normalize(front);
	// Also re-calculate the Right and Up vector
	this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}

void Camera::InitValues()
{
	this->Position = glm::vec3(0, 0, 400);
	this->WorldUp = glm::vec3(0, 1, 0);
	this->Yaw = YAW;
	this->Pitch = PITCH;
	this->MovementSpeed = SPEED;
	this->MouseSensitivity = SENSITIVTY;
	this->Zoom = ZOOM;
	this->updateCameraVectors();
}

glm::mat4 Camera ::  GetViewMatrix()
{
	return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	return Projection;
}

glm::vec3 Camera::GetPosition()
{
	return this->Position;
}
