#include "PillarBlock.h"

siegelablib::PillarBlock::PillarBlock()
{
  configurePhysics();
  
}

void siegelablib::PillarBlock::configurePhysics()
{
  rigid_body.setPosition(0.0f, 1.5f, 0.0f);
  rigid_body.setOrientation(1.0f, 0.0f, 0.0f, 0.0f);
  rigid_body.setMass(3.0f);
  cyclone::Matrix3 inertia;
  inertia.setInertiaTensorCoeffs(
      2.5f,  
      0.5f,  
      2.5f   
  );
  rigid_body.setInertiaTensor(inertia);
  rigid_body.setDamping(0.98f, 0.85f);
  rigid_body.setCanSleep(true);
  rigid_body.setAwake(true);
  collision_box.halfSize = cyclone::Vector3(0.5f, 1.5f, 0.5f);
  collision_box.body = &rigid_body;
}
