#include "shipNextMove.h"

void shipNextMove::setMove(int key, int action) {
	if (action == GLFW_PRESS) {
		switch (key) {
		case GLFW_KEY_A:
			++yaw;
			break;
		case GLFW_KEY_D:
			--yaw;
			break;
		case GLFW_KEY_W:
			++pitch;
			break;
		case GLFW_KEY_S:
			--pitch;
			break;
		case GLFW_KEY_Q:
			--roll;
			break;
		case GLFW_KEY_E:
			++roll;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			++forward;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			--forward;
			break;
		case GLFW_KEY_SPACE:
			shoot = true;
			break;
		}
	}
	if (action == GLFW_RELEASE) {
		switch (key) {
		case GLFW_KEY_A:
			--yaw;
			break;
		case GLFW_KEY_D:
			++yaw;
			break;
		case GLFW_KEY_W:
			--pitch;
			break;
		case GLFW_KEY_S:
			++pitch;
			break;
		case GLFW_KEY_Q:
			++roll;
			break;
		case GLFW_KEY_E:
			--roll;
			break;
		case GLFW_KEY_LEFT_SHIFT:
			//--forward;
			break;
		case GLFW_KEY_LEFT_CONTROL:
			//++forward;
			break;
		case GLFW_KEY_SPACE:
			shoot = false;
			break;
		}
	}
}