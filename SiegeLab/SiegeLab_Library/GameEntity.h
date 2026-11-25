#ifndef SIEGELAB_LIBRARY_GAMEENTITY_H_
#define SIEGELAB_LIBRARY_GAMEENTITY_H_

namespace siegelablib
{
    class GameEntity 
    {
    public:
        virtual ~GameEntity() = default;
        virtual void draw() = 0;
    };
}

#endif