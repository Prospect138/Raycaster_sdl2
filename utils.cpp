#include <cmath>

namespace utils {

void NormalizeAngle(double& angle){
    if (360.0 < angle) {
        angle -= 360 * (int(angle) / 360);
    }
    else if (angle < 0.0){
        angle += 360 * ceil(abs(angle) / 360);
    }
}
}