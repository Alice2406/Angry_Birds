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
        clear();
    }

    void SceneObjects::clear() {
        for (int i = 0; i < MaxEntities; ++i) {
            if (entities_[i] != nullptr) {
                delete entities_[i];
                entities_[i] = nullptr;
            }
            toRemove_[i] = false;
            ++generations_[i];
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
        if (id.slot < 0 || id.slot > MaxEntities || entities_[id.slot] == nullptr 
            || generations_[id.slot] != id.generation)
            return nullptr;

        return entities_[id.slot];
    }

    void SceneObjects::markForRemoval(EntityId id)
    {
        if (id.slot >= 0 && id.slot < MaxEntities && entities_[id.slot] != nullptr 
            && generations_[id.slot] == id.generation) 
                toRemove_[id.slot] = true;
    }

    void siegelablib::SceneObjects::flushRemovals()
    {
        for (int i = 0; i < MaxEntities; ++i) 
        {
            if (toRemove_[i] && entities_[i] != nullptr) 
            {
                delete entities_[i];
                entities_[i] = nullptr;
                ++generations_[i];
                toRemove_[i] = false;
            }
        }
    }

    void SceneObjects::updateAll(float dt)
    {
        for (int i = 0; i < MaxEntities; ++i) 
            if (entities_[i] != nullptr) 
                entities_[i]->update(dt);
    }

    void SceneObjects::drawAll() 
    {
        for (int i = 0; i < MaxEntities; ++i) 
            if (entities_[i] != nullptr) 
                entities_[i]->draw();
    }
}
