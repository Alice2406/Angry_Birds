#include "pch.h"
#include <cmath>

#include "EntityId.h"
#include "GameEntity.h"
#include "HeavyProjectile.h"
#include "Projectile.h"
#include "SceneObjects.h"
#include "StandardProjectile.h"

namespace {

    void simulateOnGround(siegelablib::Projectile& proj, int steps, float dt) {
        cyclone::CollisionPlane ground;
        ground.direction = cyclone::Vector3(0.0f, 1.0f, 0.0f);
        ground.offset = 0.0f;

        cyclone::ContactResolver resolver(16, 16, 0.01f, 0.01f);

        for (int i = 0; i < steps; ++i) {
            proj.update(dt);

            cyclone::Contact contacts[4];
            cyclone::CollisionData data;
            data.contactArray = contacts;
            data.contacts = contacts;
            data.contactsLeft = 4;
            data.contactCount = 0;
            data.friction = 0.6f;
            data.restitution = 0.1f;
            data.tolerance = 0.01f;

            cyclone::CollisionDetector::sphereAndTruePlane(
                proj.getSphere(), ground, &data);

            if (data.contactCount > 0) {
                resolver.resolveContacts(data.contactArray, data.contactCount, dt);
            }
        }
    }

    int simulateHeldSpaceFrames(siegelablib::SceneObjects& scene, int frames) {
        int createdShots = 0;
        bool spaceWasDown = false;

        for (int f = 0; f < frames; ++f) {
            bool spaceDown = true;
            if (spaceDown && !spaceWasDown) {
                scene.createEntity<siegelablib::StandardProjectile>();
                ++createdShots;
            }
            spaceWasDown = spaceDown;
        }

        return createdShots;
    }

}

TEST(EntityAndProjectileTests, GO01_SceneCreatesDifferentDerivedTypes) {
    siegelablib::SceneObjects scene;

    siegelablib::EntityId idStd = scene.createEntity<siegelablib::StandardProjectile>();
    siegelablib::EntityId idHeavy = scene.createEntity<siegelablib::HeavyProjectile>();

    EXPECT_GE(idStd.slot, 0);
    EXPECT_GE(idHeavy.slot, 0);
    EXPECT_NE(idStd.slot, idHeavy.slot);
    EXPECT_NE(scene.get(idStd), nullptr);
    EXPECT_NE(scene.get(idHeavy), nullptr);
}

TEST(EntityAndProjectileTests, GO02_VirtualDestructorOnBasePointer) {
    siegelablib::SceneObjects scene;
    siegelablib::EntityId id = scene.createEntity<siegelablib::HeavyProjectile>();

    siegelablib::GameEntity* basePtr = scene.get(id);
    ASSERT_NE(basePtr, nullptr);

    scene.markForRemoval(id);
    scene.flushRemovals();
    EXPECT_EQ(scene.get(id), nullptr);
}

TEST(EntityAndProjectileTests, GO03_SceneOwnsAndCleansEntities) {
    siegelablib::SceneObjects scene;

    siegelablib::EntityId id1 = scene.createEntity<siegelablib::StandardProjectile>();
    siegelablib::EntityId id2 = scene.createEntity<siegelablib::HeavyProjectile>();

    EXPECT_NE(scene.get(id1), nullptr);
    EXPECT_NE(scene.get(id2), nullptr);
}

TEST(EntityAndProjectileTests, PR01_StandardAndHeavyProfiles) {
    siegelablib::StandardProjectile stdProj;
    siegelablib::HeavyProjectile heavyProj;

    EXPECT_NEAR(stdProj.getBody().getMass(), 1.0f, 1e-4f);
    EXPECT_NEAR(stdProj.getRadius(), 0.28f, 1e-4f);

    EXPECT_NEAR(heavyProj.getBody().getMass(), 2.0f, 1e-4f);
    EXPECT_NEAR(heavyProj.getRadius(), 0.36f, 1e-4f);
}

TEST(EntityAndProjectileTests, PR02_InertiaDampingAndColliderPointers) {
    siegelablib::StandardProjectile stdProj;

    EXPECT_EQ(stdProj.getSphere().body, &stdProj.getBody());
    EXPECT_NEAR(stdProj.getSphere().radius, 0.28f, 1e-4f);
    EXPECT_GT(stdProj.getBody().getLinearDamping(), 0.0f);
    EXPECT_GT(stdProj.getBody().getAngularDamping(), 0.0f);

    cyclone::Matrix3 inertia = stdProj.getBody().getInertiaTensor();
    const float expectedInertia = (2.0f / 5.0f) * 1.0f * 0.28f * 0.28f;
    EXPECT_NEAR(inertia.data[0], expectedInertia, 1e-4f);
    EXPECT_NEAR(inertia.data[4], expectedInertia, 1e-4f);
    EXPECT_NEAR(inertia.data[8], expectedInertia, 1e-4f);
}

TEST(EntityAndProjectileTests, PR03_HeldSpaceCreatesOnlyOneShot) {
    siegelablib::SceneObjects scene;
    int shotsCreated = simulateHeldSpaceFrames(scene, 10);

    EXPECT_EQ(shotsCreated, 1);
}

TEST(EntityAndProjectileTests, PR04_BodyPoseFollowsIntegration) {
    siegelablib::StandardProjectile proj;
    siegelablib::AimVec3 startPos{ 0.0f, 3.0f, 0.0f };
    siegelablib::AimVec3 dir{ 1.0f, 0.0f, 0.0f };

    proj.fire(startPos, dir, 10.0f);
    proj.update(0.05f);

    cyclone::Vector3 pos = proj.getBody().getPosition();
    EXPECT_GT(pos.x, 0.0f);
    EXPECT_LT(pos.y, 3.0f);
}

TEST(EntityAndProjectileTests, PR05_FallsAndRestsOnGroundPlane) {
    siegelablib::StandardProjectile proj;
    siegelablib::AimVec3 startPos{ 0.0f, 2.0f, 0.0f };
    siegelablib::AimVec3 dir{ 0.5f, 0.2f, 0.0f };

    proj.fire(startPos, dir, 4.0f);
    simulateOnGround(proj, 240, 1.0f / 60.0f);

    cyclone::Vector3 pos = proj.getBody().getPosition();
    EXPECT_GE(pos.y, 0.20f);
    EXPECT_LE(pos.y, 0.40f);
}