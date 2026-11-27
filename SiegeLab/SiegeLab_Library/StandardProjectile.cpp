#include "StandardProjectile.h"
#include "raylib.h"

namespace siegelablib {

    StandardProjectile::StandardProjectile() 
    {
        configurePhysics();
    }

    void StandardProjectile::configurePhysics() 
    {
        setupBodyAndSphere(1.0f, 0.28f);
    }
	void StandardProjectile::draw() {}

}