#pragma once
#include "Block.h"

namespace siegelablib
{
  

class PillarBlock : public siegelablib::Block
{
public:
  PillarBlock();
  void configurePhysics();
};
}