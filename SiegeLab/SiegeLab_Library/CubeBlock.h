#pragma once
#include "Block.h"

namespace siegelablib{
class CubeBlock :public siegelablib::Block
{
public:
  void configurePhysics();
  
};
}