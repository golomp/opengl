#pragma once
#ifndef SHIPSTATE_H
#define SHIPSTATE_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "shipNextMove.h"
#include "constants.h"
#include "bullet.h"
class shipState {
public:
	float pitch_angle = 0;
	float roll_angle = 0;
	float yaw_angle = 0;
	float forward = 0;
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraRight = glm::vec3(1.0f, 0.0f, 0.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
	glm::vec3 direction;
	Bullet bullets[1000];
	int howManyBullets = 0;

	void transform_camera_vectors(glm::mat4 rotation_mat);
	void moveCamera(double time, shipNextMove nextmove);
	void shoot();
};

#endif	