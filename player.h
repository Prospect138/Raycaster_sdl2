#pragma once

#include "utils.h"

struct Player {
	Point coordinate;
	double rotation_angle;
	double fov;

	double speed = 0.2;
};
