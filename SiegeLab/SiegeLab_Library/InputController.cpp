#include "InputController.h"
#include "Launcher.h"

namespace siegelablib {

    void InputController::onKeyPressed(int key, Launcher& launcher) {
        launcher.chooseKindFromKey(key);
    }

    void InputController::onAimInput(float angleXDeg, float angleYDeg, float speed, Launcher& launcher) {
        launcher.setAim(angleXDeg, angleYDeg, speed);
    }

}