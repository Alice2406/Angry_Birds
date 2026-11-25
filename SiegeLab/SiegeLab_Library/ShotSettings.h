#pragma once

namespace siegelablib {

    enum class ShotKind {
        Standard,
        Heavy
    };

    struct AimVec3 {
        float x = 0.0f;
        float y = 0.0f;
        float z = 0.0f;
    };

    struct ShotSettings {
        float angleXDeg = 0.0f;
        float angleYDeg = 30.0f;
        float speed = 16.0f;
        ShotKind kind = ShotKind::Standard;
    };

}