#include "player.h"

Player::Player(Point coordinates, double rotation, int view)
{
    coordinate = coordinates;
    rotation_angle = rotation;
    fov = view;
}

void Player::Rotate(double angle)
{
    if (angle > 360) {
        std::cerr << "error when rotating\n";
    }
    rotation_angle += angle;
    if (rotation_angle > 359.0) {
        rotation_angle -= 360.0;
    }
    else if (rotation_angle < 0.0) {
        rotation_angle += 360.0;
    }
    //std::cerr << rotation_angle << "\n";
}

double Player::GetRotationAngle()
{
    return rotation_angle;
}
