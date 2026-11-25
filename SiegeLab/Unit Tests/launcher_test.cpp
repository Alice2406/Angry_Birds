#include "pch.h"
#include <cmath>

#include "InputController.h"
#include "Launcher.h"
#include "ShotSettings.h"

namespace {

    float vectorLength(const siegelablib::AimVec3& v) {
        return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    }

    float fakeCameraYaw(float yawDeg) {
        return yawDeg;
    }

}

TEST(LauncherTests, L01_ZeroAzimuthHasNoZComponent) {
    siegelablib::Launcher launcher;
    launcher.setAim(0.0f, 30.0f, 16.0f);

    siegelablib::AimVec3 dir = launcher.getDirection();

    EXPECT_NEAR(dir.x, 0.8660f, 1e-4f);
    EXPECT_NEAR(dir.y, 0.5000f, 1e-4f);
    EXPECT_NEAR(dir.z, 0.0000f, 1e-4f);
}

TEST(LauncherTests, L02_DirectionIsNormalized) {
    siegelablib::Launcher launcher;

    launcher.setAim(30.0f, 30.0f, 16.0f);
    siegelablib::AimVec3 dir1 = launcher.getDirection();
    EXPECT_NEAR(dir1.x, 0.7500f, 1e-4f);
    EXPECT_NEAR(dir1.y, 0.5000f, 1e-4f);
    EXPECT_NEAR(dir1.z, 0.4330f, 1e-4f);
    EXPECT_NEAR(vectorLength(dir1), 1.0f, 1e-4f);

    launcher.setAim(-30.0f, 45.0f, 16.0f);
    siegelablib::AimVec3 dir2 = launcher.getDirection();
    EXPECT_NEAR(dir2.x, 0.6124f, 1e-4f);
    EXPECT_NEAR(dir2.y, 0.7071f, 1e-4f);
    EXPECT_NEAR(dir2.z, -0.3536f, 1e-4f);
    EXPECT_NEAR(vectorLength(dir2), 1.0f, 1e-4f);
}

TEST(LauncherTests, L03_ParametersAreClamped) {
    siegelablib::Launcher launcher;
    launcher.setAim(45.0f, -10.0f, 30.0f);

    EXPECT_NEAR(launcher.getAngleXDeg(), 30.0f, 1e-4f);
    EXPECT_NEAR(launcher.getAngleYDeg(), 5.0f, 1e-4f);
    EXPECT_NEAR(launcher.getSpeed(), 22.0f, 1e-4f);
}

TEST(LauncherTests, L04_AimIsIndependentFromCamera) {
    siegelablib::Launcher launcher;
    launcher.setAim(20.0f, 30.0f, 16.0f);

    float camYaw = fakeCameraYaw(0.0f);
    (void)camYaw;
    siegelablib::AimVec3 dirBefore = launcher.getDirection();

    camYaw = fakeCameraYaw(120.0f);
    (void)camYaw;
    siegelablib::AimVec3 dirAfter = launcher.getDirection();

    EXPECT_NEAR(dirBefore.x, 0.8138f, 1e-4f);
    EXPECT_NEAR(dirBefore.y, 0.5000f, 1e-4f);
    EXPECT_NEAR(dirBefore.z, 0.2962f, 1e-4f);

    EXPECT_NEAR(std::abs(dirAfter.x - dirBefore.x), 0.0f, 1e-4f);
    EXPECT_NEAR(std::abs(dirAfter.y - dirBefore.y), 0.0f, 1e-4f);
    EXPECT_NEAR(std::abs(dirAfter.z - dirBefore.z), 0.0f, 1e-4f);
}

TEST(LauncherTests, L05_SelectsConcreteProjectileKind) {
    siegelablib::Launcher launcher;
    siegelablib::InputController input;

    input.onAimInput(0.0f, 30.0f, 16.0f, launcher);

    input.onKeyPressed(2, launcher);
    siegelablib::ShotSettings shot2 = launcher.makeShotSettings();
    EXPECT_EQ(shot2.kind, siegelablib::ShotKind::Heavy);
    EXPECT_NEAR(shot2.speed, 16.0f, 1e-4f);

    input.onKeyPressed(1, launcher);
    siegelablib::ShotSettings shot1 = launcher.makeShotSettings();
    EXPECT_EQ(shot1.kind, siegelablib::ShotKind::Standard);
    EXPECT_NEAR(shot1.speed, 16.0f, 1e-4f);
}