#include "HeavyProjectile.h"
#include "raylib.h"

namespace siegelablib {

    HeavyProjectile::HeavyProjectile() 
    {
        configurePhysics();
    }

    void HeavyProjectile::configurePhysics() 
    {
        setupBodyAndSphere(2.0f, 0.36f);
    }
	void HeavyProjectile::draw()
	{}
}