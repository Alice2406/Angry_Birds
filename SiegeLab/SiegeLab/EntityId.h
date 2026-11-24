#pragma once

namespace siegelab {

    struct EntityId {
        int slot = -1;
        int generation = 0;

        bool isValid()
        {
            return slot >= 0 && generation > 0;
        }
    };

}