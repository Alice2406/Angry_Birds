#pragma once

#include "EntityId.h"
#include <array>
#include <optional>

namespace siegelab {
    template <typename T, int Capacity>
    class ObjectPool {
    public:
        EntityId create(const T& item) 
        {
            for (int i = 0; i < Capacity; ++i) 
            {
                if (!slots_[i].value.has_value()) 
                {
                    slots_[i].value = item;
                    return EntityId{ i, slots_[i].generation };
                }
            }
            return EntityId{ -1, 0 };
        }

        T* get(EntityId id) 
        {
            if (!isValid(id))
                return nullptr;
            return &slots_[id.slot].value.value();
        }

        bool destroy(EntityId id) 
        {
            if (!isValid(id))
                return false;
            slots_[id.slot].value.reset();
            slots_[id.slot].generation++;
            return true;
        }

        void clear() {
            for (int i = 0; i < Capacity; ++i) 
            {
                if (slots_[i].value.has_value()) 
                {
                    slots_[i].value.reset();
                    slots_[i].generation++;
                }
            }
        }

        template <typename Func>
        void forEach(Func func) 
        {
            for (int i = 0; i < Capacity; ++i) 
                if (slots_[i].value.has_value()) 
                    func(slots_[i].value.value());
        }

    private:
        struct Slot 
        {
            std::optional<T> value;
            int generation = 1;
        };

        bool isValid(EntityId id) const 
        {
            if (id.slot < 0 || id.slot >= Capacity || slots_[id.slot].generation != id.generation || !slots_[id.slot].value.has_value())
                return false;
            return true;
        }

        std::array<Slot, Capacity> slots_;
    };

}