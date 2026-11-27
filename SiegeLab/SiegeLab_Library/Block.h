#pragma once
#include "GameEntity.h"
#include "cyclone/collide_fine.h"
#include "cyclone/world.h"

namespace  siegelablib
{
  

class Block : public siegelablib::GameEntity
{
public:
  cyclone::CollisionBox collision_box;
  
  void draw() override;
  void registerPhysicsWorld(cyclone::World);
};
}
