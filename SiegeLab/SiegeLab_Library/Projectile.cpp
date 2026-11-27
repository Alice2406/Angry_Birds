#include "Projectile.h"
#include "raylib.h"
#include <cmath>

namespace siegelablib {

    Projectile::Projectile()
        : radius(0.28f) {
        sphere.body = &body;
        sphere.radius = radius;
        sphere.calculateInternals();
    }

    Projectile::~Projectile() {
    }

    void Projectile::setupBodyAndSphere(float mass, float radiusprojectile) {
        radius = radiusprojectile;

        body.setMass(mass);
        body.setDamping(0.99f, 0.8f);
        body.setAcceleration(0.0f, -9.81f, 0.0f);

        const float inertiaDiag = (2.0f / 5.0f) * mass * radius * radius;
        cyclone::Matrix3 inertia;
        inertia.setDiagonal(inertiaDiag, inertiaDiag, inertiaDiag);
        body.setInertiaTensor(inertia);

        body.setAwake(true);
        body.setCanSleep(true);
        body.calculateDerivedData();

        sphere.body = &body;
        sphere.radius = radius;
        sphere.calculateInternals();
    }

    void Projectile::fire(const AimVec3& startPos, const AimVec3& direction, float speed) {
        const float len = std::sqrt(
            direction.x * direction.x +
            direction.y * direction.y +
            direction.z * direction.z);

        float dirX = 0.0f;
        float dirY = 0.0f;
        float dirZ = 0.0f;
        if (len > 0.0f) {
            dirX = direction.x / len;
            dirY = direction.y / len;
            dirZ = direction.z / len;
        }

        body.setPosition(startPos.x, startPos.y, startPos.z);
        body.setVelocity(dirX * speed, dirY * speed, dirZ * speed);
        body.setRotation(0.0f, 0.0f, 0.0f);
        body.setAcceleration(0.0f, -9.81f, 0.0f);
        body.clearAccumulators();
        body.setAwake(true);
        body.calculateDerivedData();

        sphere.body = &body;
        sphere.radius = radius;
        sphere.calculateInternals();
    }

    void Projectile::update(float dt) {
        body.integrate(dt);
        sphere.calculateInternals();
    }

    cyclone::CollisionSphere& Projectile::getSphere() {
        return sphere;
    }

    float Projectile::getRadius() const {
        return radius;
    }

}