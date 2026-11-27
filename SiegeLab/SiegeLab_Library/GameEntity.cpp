#include "GameEntity.h"

namespace siegelablib {

    GameEntity::GameEntity() 
    {
        body.setPosition(0.0f, 0.0f, 0.0f);
        body.setVelocity(0.0f, 0.0f, 0.0f);
        body.setAcceleration(0.0f, -9.81f, 0.0f);
        body.setDamping(0.99f, 0.8f);
        body.setAwake(true);
        body.setCanSleep(true);
        body.calculateDerivedData();
    }

    void GameEntity::update(float dt) 
    {
        body.integrate(dt);
    }

    cyclone::RigidBody& GameEntity::getBody() 
    {
        return body;
    }

    const cyclone::RigidBody& GameEntity::getBody() const 
    {
        return body;
    }
}