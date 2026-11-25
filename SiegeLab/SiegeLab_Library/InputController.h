#pragma once

namespace siegelablib {

    class Launcher;

    class InputController {
    public:
        void onKeyPressed(int key, Launcher& launcher);
        void onAimInput(float angleXDeg, float angleYDeg, float speed, Launcher& launcher);
    };

}