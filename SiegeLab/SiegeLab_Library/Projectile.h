#pragma once

#include "GameEntity.h"
#include "ShotSettings.h"
#include "cyclone/collide_fine.h"

namespace siegelablib {

    class Projectile : public GameEntity {
    public:
        Projectile();
        virtual ~Projectile();

        void fire(const AimVec3& startPos, const AimVec3& direction, float speed);
        void update(float dt) override;
        void draw() = 0;

        cyclone::CollisionSphere& getSphere();
        float getRadius() const;
    protected:
        void setupBodyAndSphere(float mass, float radius);
        virtual void configurePhysics() = 0;

        cyclone::CollisionSphere sphere;
        float radius;
    };

}