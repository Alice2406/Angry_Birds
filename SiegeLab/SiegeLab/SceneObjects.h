#ifndef SIEGELAB_LIBRARY_SCENEOBJECTS_H_
#define SIEGELAB_LIBRARY_SCENEOBJECTS_H_

#include "EntityId.h"
#include "GameEntity.h"

namespace siegelablib
{
    class SceneObjects 
    {
    public:
        static const int MaxEntities = 64;

        SceneObjects();
        ~SceneObjects();

        EntityId createEntity(GameEntity* entity);
        GameEntity* get(EntityId id);
        void flushRemovals();

    private:
        GameEntity* entities_[MaxEntities];
        int generations_[MaxEntities];
        bool toRemove_[MaxEntities];
    };

}

#endif