#pragma once

#include "Projectile.h"

namespace siegelablib {

    class HeavyProjectile : public Projectile {
    public:
        HeavyProjectile();
        void draw() override;

    protected:
        void configurePhysics() override;
    };

}