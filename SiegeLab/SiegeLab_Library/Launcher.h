#pragma once

#include "ShotSettings.h"

namespace siegelablib {

    class Launcher {
    public:
        static const float MinAngleXDeg;
        static const float MaxAngleXDeg;
        static const float MinAngleYDeg;
        static const float MaxAngleYDeg;
        static const float MinSpeed;
        static const float MaxSpeed;

        Launcher();

        void setAim(float angleXDeg, float angleYDeg, float speed);
        void chooseKindFromKey(int key);

        float getAngleXDeg() const;
        float getAngleYDeg() const;
        float getSpeed() const;
        ShotKind getKind() const;

        AimVec3 getDirection() const;
        ShotSettings makeShotSettings() const;

    private:
        float angleXDeg_;
        float angleYDeg_;
        float speed_;
        ShotKind kind_;
    };

}