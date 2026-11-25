#ifndef SIEGELAB_LIBRARY_SCENEOBJECTS_H_
#define SIEGELAB_LIBRARY_SCENEOBJECTS_H_

#include <type_traits>
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

        template <typename T>
        EntityId createEntity() 
        {
            static_assert(std::is_base_of<GameEntity, T>::value, "T doit heriter de GameEntity");
            return createEntity(new T());
        }

        GameEntity* get(EntityId id);
        void markForRemoval(EntityId id);
        void flushRemovals();

    private:
        GameEntity* entities_[MaxEntities];
        int generations_[MaxEntities];
        bool toRemove_[MaxEntities];
    };

}

#endif