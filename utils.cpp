#include <cmath>
#include "utils.h"

namespace utils {

void NormalizeAngle(double& angle){
    if (angle > 360.0) {
        angle -= 360.0;
    }
    else if (angle < 0.0){
        angle += 360.0;
    }
}

}