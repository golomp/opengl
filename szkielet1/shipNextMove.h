#pragma once
#ifndef SHIPNEXT_H
#define SHIPNEXT_H

#include <GLFW/glfw3.h>


class shipNextMove {
public:
	float pitch = 0;
	float roll = 0;
	float yaw = 0;
	float forward = 0;
	bool shoot = false;

	void setMove(int key, int action);
};

#endif	