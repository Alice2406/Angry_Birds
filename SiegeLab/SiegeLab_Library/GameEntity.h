#ifndef SIEGELAB_LIBRARY_GAMEENTITY_H_
#define SIEGELAB_LIBRARY_GAMEENTITY_H_
#include "cyclone/body.h"

namespace siegelablib
{
    class GameEntity 
    {
    public:
        virtual ~GameEntity() = default;
        virtual void draw() = 0;
        cyclone::RigidBody rigid_body;
    };
}

#endif