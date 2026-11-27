#ifndef SIEGELAB_LIBRARY_GAMEENTITY_H_
#define SIEGELAB_LIBRARY_GAMEENTITY_H_
#include "cyclone/body.h"

namespace siegelablib
{
    class GameEntity 
    {
    public:
		GameEntity();
        virtual ~GameEntity() = default;
        virtual void update(float dt);
        virtual void draw() = 0;
        cyclone::RigidBody& getBody();
		const cyclone::RigidBody& getBody() const;

    protected:
        cyclone::RigidBody body;
    };
}

#endif