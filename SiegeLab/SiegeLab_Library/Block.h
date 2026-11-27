#pragma once
#include "GameEntity.h"
#include "cyclone/collide_fine.h"
#include "cyclone/world.h"

namespace  siegelablib
{
  

class Block : public GameEntity
{
public:
  cyclone::CollisionBox collision_box;
  
  virtual  void draw();
  virtual void registerPhysicsWorld(cyclone::World& world) = 0;
};
}
