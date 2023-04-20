#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>

enum Camera_Movement {
	C_FORWARD,
	C_BACKWARD,
	C_LEFT,
	C_RIGHT
};

class Camera {
public:
	float yaw, pitch;

	glm::vec3 position;

	glm::vec3 UP;
	glm::vec3 RIGHT;
	glm::vec3 FRONT;

	glm::vec3 wordUP = {0.0f, 1.0f, 0.0f};

	float SPEED = 5.25f;

	float sensibility = 0.1f;
	float zoom = 45.0f;

public:
	Camera(glm::vec3 nPosition, glm::vec3 nUP, float nYaw = -90.0f, float nPitch = 0.0f);

	void handleMouse(float xOffset, float yOffset);
	void handleScroll(float yOffset);
	void handleKeyboard(float delta, Camera_Movement direction);

	inline glm::mat4 getViewMatrix() { return glm::lookAt(position, position + FRONT, UP); }
private:
	void update();

};