#include "shipState.h"
#include <iostream>

void shipState::transform_camera_vectors(glm::mat4 rotation_mat) {
	cameraRight = glm::vec3(rotation_mat * glm::vec4(cameraRight, 0.0f));
	cameraUp = glm::vec3(rotation_mat * glm::vec4(cameraUp, 0.0f));
	cameraFront = glm::vec3(rotation_mat * glm::vec4(cameraFront, 0.0f));
}

void shipState::moveCamera(double time, shipNextMove move) {
	glm::mat4 M = glm::mat4(1.0f);

	double forward_speed = 5l * time;

	GLfloat cameraSpeed = 1l * time  * move.forward;

	if (move.pitch == 1) {
		if (pitch_angle < 0.05) {
			pitch_angle += 0.04l * time;
		}
		if (pitch_angle < 0) {
			pitch_angle += 0.06l * time;
		}
	}
	else if (move.pitch == -1) {
		if (pitch_angle > -0.05) {
			pitch_angle -= 0.04l * time;
		}
		if (pitch_angle > 0) {
			pitch_angle -= 0.06l * time;
		}
	}
	else {
		pitch_angle /= 1.1l;
		if (abs(pitch_angle) < 0.00000000002) {
			pitch_angle = 0;
		}
	}


	if (move.roll == 1) {
		if (roll_angle < 0.05) {
			roll_angle += 0.04l * time;
		}
		if (roll_angle < 0) {
			roll_angle += 0.06l * time;
		}
	}
	else if (move.roll == -1) {
		if (roll_angle > -0.05) {
			roll_angle -= 0.04l * time;
		}
		if (roll_angle > 0) {
			roll_angle -= 0.06l * time;
		}
	}
	else {
		roll_angle /= 1.1l;
		if (abs(roll_angle) < 0.00000000002) {
			roll_angle = 0;
		}
	}

	if (move.yaw == 1) {
		if (yaw_angle < 0.05) {
			yaw_angle += 0.04l * time;
		}
		if (yaw_angle < 0) {
			yaw_angle += 0.06l * time;
		}
	}
	else if (move.yaw == -1) {
		if (yaw_angle > -0.05) {
			yaw_angle -= 0.04l * time;
		}
		if (yaw_angle > 0) {
			yaw_angle -= 0.06l * time;
		}
	}
	else {
		yaw_angle /= 1.1l;
		if (abs(yaw_angle) < 0.00000000002) {
			yaw_angle = 0;
		}
	}

	glm::mat4 rotationMat(1);
	rotationMat = glm::rotate(rotationMat, pitch_angle, cameraRight);
	transform_camera_vectors(rotationMat);


	rotationMat = glm::mat4(1);
	rotationMat = glm::rotate(rotationMat, roll_angle, cameraFront);
	transform_camera_vectors(rotationMat);


	rotationMat = glm::mat4(1);
	rotationMat = glm::rotate(rotationMat, yaw_angle, cameraUp);
	transform_camera_vectors(rotationMat);


	if (move.forward != 0) {
		cameraPos += cameraFront * cameraSpeed;
	}

	view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
}

void shipState::shoot() {
	bullets[howManyBullets].direction = cameraFront;
	glm::mat4 mt1 = glm::mat4(1.0f);
	glm::mat4 mt2 = glm::mat4(1.0f);
	mt1 = translate(mt1, cameraPos);
	mt2 = translate(mt2, cameraPos);
	mt1 = translate(mt1, cameraRight);
	mt2 = translate(mt2, cameraRight * -1.f);

	if (bullets[howManyBullets].direction.x >= 0) {
		mt1 = rotate(mt1, PI / 2.0f + asin(cameraFront.z / sqrt(cameraFront.z*cameraFront.z + cameraFront.x*cameraFront.x)), glm::vec3(0, -1, 0));
		mt2 = rotate(mt2, PI / 2.0f + asin(cameraFront.z / sqrt(cameraFront.z*cameraFront.z + cameraFront.x*cameraFront.x)), glm::vec3(0, -1, 0));
	}
	else {
		mt1 = rotate(mt1, -1.57f - asin(cameraFront.z / sqrt(cameraFront.z*cameraFront.z + cameraFront.x*cameraFront.x)), glm::vec3(0, -1, 0));
		mt2 = rotate(mt2, -1.57f - asin(cameraFront.z / sqrt(cameraFront.z*cameraFront.z + cameraFront.x*cameraFront.x)), glm::vec3(0, -1, 0));
	}
	mt1 = rotate(mt1, asin(bullets[howManyBullets].direction.y), glm::vec3(1, 0, 0));
	mt2 = rotate(mt2, asin(bullets[howManyBullets].direction.y), glm::vec3(1, 0, 0));

	bullets[howManyBullets].m = mt1;
	bullets[howManyBullets].alive = true;
	++howManyBullets;
	bullets[howManyBullets].m = mt2;
	bullets[howManyBullets].alive = true;
	++howManyBullets;
}