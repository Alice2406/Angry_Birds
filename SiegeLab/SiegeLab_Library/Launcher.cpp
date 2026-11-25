#include "Launcher.h"
#include <cmath>

namespace {

    float clampFloat(float value, float minVal, float maxVal) {
        if (value < minVal) {
            return minVal;
        }
        if (value > maxVal) {
            return maxVal;
        }
        return value;
    }

    float toRadians(float degrees) {
        const float pi = 3.14159265358979323846f;
        return degrees * (pi / 180.0f);
    }

}

namespace siegelablib {

    const float Launcher::MinAngleXDeg = -30.0f;
    const float Launcher::MaxAngleXDeg = 30.0f;
    const float Launcher::MinAngleYDeg = 5.0f;
    const float Launcher::MaxAngleYDeg = 55.0f;
    const float Launcher::MinSpeed = 8.0f;
    const float Launcher::MaxSpeed = 22.0f;

    Launcher::Launcher()
        : angleXDeg_(0.0f)
        , angleYDeg_(30.0f)
        , speed_(16.0f)
        , kind_(ShotKind::Standard) {
    }

    void Launcher::setAim(float angleXDeg, float angleYDeg, float speed) {
        angleXDeg_ = clampFloat(angleXDeg, MinAngleXDeg, MaxAngleXDeg);
        angleYDeg_ = clampFloat(angleYDeg, MinAngleYDeg, MaxAngleYDeg);
        speed_ = clampFloat(speed, MinSpeed, MaxSpeed);
    }

    void Launcher::chooseKindFromKey(int key) {
        if (key == 2) {
            kind_ = ShotKind::Heavy;
        }
        else if (key == 1) {
            kind_ = ShotKind::Standard;
        }
    }

    float Launcher::getAngleXDeg() const {
        return angleXDeg_;
    }

    float Launcher::getAngleYDeg() const {
        return angleYDeg_;
    }

    float Launcher::getSpeed() const {
        return speed_;
    }

    ShotKind Launcher::getKind() const {
        return kind_;
    }

    AimVec3 Launcher::getDirection() const {
        const float radX = toRadians(angleXDeg_);
        const float radY = toRadians(angleYDeg_);

        const float cosY = std::cos(radY);
        AimVec3 dir;
        dir.x = cosY * std::cos(radX);
        dir.y = std::sin(radY);
        dir.z = cosY * std::sin(radX);

        const float length = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
        if (length > 0.0f) {
            dir.x /= length;
            dir.y /= length;
            dir.z /= length;
        }
        return dir;
    }

    ShotSettings makeShotSettingsHelper(float angleXDeg, float angleYDeg, float speed, ShotKind kind) {
        ShotSettings settings;
        settings.angleXDeg = angleXDeg;
        settings.angleYDeg = angleYDeg;
        settings.speed = speed;
        settings.kind = kind;
        return settings;
    }

    ShotSettings Launcher::makeShotSettings() const {
        return makeShotSettingsHelper(angleXDeg_, angleYDeg_, speed_, kind_);
    }

}