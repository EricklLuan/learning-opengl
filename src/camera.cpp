#include "camera.hpp"

#include <iostream>

Camera::Camera(glm::vec3 nPosition, glm::vec3 nUP, float nYaw, float nPitch):
position(nPosition), wordUP(nUP), FRONT({0.0f, 0.0f, -1.0f}), yaw(nYaw), pitch(nPitch)
{
	update();
}

void Camera::handleMouse(float xOffset, float yOffset)
{
	xOffset *= sensibility;
	yOffset *= sensibility;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0f) pitch = 89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	update();

}

void Camera::handleScroll(float yOffset) 
{
	std::cout << zoom << "\n";
	zoom -= (float)yOffset;
	if (zoom < 1.0f) zoom = 1.0f;
	if (zoom > 45.0f) zoom = 45.0f;

}	

void Camera::handleKeyboard(float delta, Camera_Movement direction) 
{
	float velocity = SPEED * delta;
	if (direction == C_FORWARD) position += FRONT * velocity;
	if (direction == C_BACKWARD) position -= FRONT * velocity;
	if (direction == C_LEFT) position -= RIGHT * velocity;
	if (direction == C_RIGHT) position += RIGHT * velocity;
}

void Camera::update()
{
	glm::vec3 front;
	front.x = std::cos(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	front.y = std::sin(glm::radians(pitch));
	front.z = std::sin(glm::radians(yaw)) * std::cos(glm::radians(pitch));
	FRONT = glm::normalize(front);

	RIGHT = glm::normalize(glm::cross(FRONT, wordUP));
	UP = glm::normalize(glm::cross(RIGHT, FRONT));

}
