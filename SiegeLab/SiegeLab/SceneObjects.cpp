#include "SceneObjects.h"

namespace siegelablib
{
    SceneObjects::SceneObjects()
    {
        for (int i = 0; i < MaxEntities; i++)
        {
            entities_[i] = nullptr;
            generations_[i] = 1;
            toRemove_[i] = false;
        }
    }

    SceneObjects::~SceneObjects()
    {
        for (int i = 0; i < MaxEntities; i++)
        {
            delete entities_[i];
            entities_[i] = nullptr;
        }
    }

    EntityId siegelablib::SceneObjects::createEntity(GameEntity* entity)
    {
        if (entity == nullptr)
            return EntityId{ -1, 0 };
        for (int i = 0; i < MaxEntities; i++)
        {
            if (entities_[i] == nullptr)
            {
                entities_[i] = entity;
                toRemove_[i] = false;
                return (EntityId{ i, generations_[i] });
            }
        }
        return EntityId{ -1, 0 };
    }

    GameEntity* siegelablib::SceneObjects::get(EntityId id)
    {
        if (id.slot < 0 || id.slot > MaxEntities || entities_[id.slot] == nullptr || generations_[id.slot] != id.generation)
            return nullptr;

        return entities_[id.slot];
    }

    void siegelablib::SceneObjects::flushRemovals()
    {
        for (int i = 0; i < MaxEntities; ++i) {
            if (toRemove_[i] && entities_[i] != nullptr) {
                delete entities_[i];
                entities_[i] = nullptr;
                ++generations_[i];
                toRemove_[i] = false;
            }
        }
    }
}
