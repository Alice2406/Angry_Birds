#pragma once

#include "Projectile.h"

namespace siegelablib {

    class StandardProjectile : public Projectile {
    public:
        StandardProjectile();
        void draw() override;

    protected:
        void configurePhysics() override;
    };

}