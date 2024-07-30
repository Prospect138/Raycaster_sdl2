#pragma once

#include <iostream>
#include "utils.h"

class Player {
public:
	Player(Point coordinates, double rotation, int view);
	Point coordinate;
	int fov;
	double speed = 0.1;
	void Rotate(double angle);
	double GetRotationAngle();
private:
	double rotation_angle;
};
