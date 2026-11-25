#ifndef SIEGELAB_LIBRARY_ENTITY_H_
#define SIEGELAB_LIBRARY_ENTITY_H_

namespace siegelablib
{
    struct EntityId
    {
        int slot = -1;
        int generation = 0;

        bool isValid()
        {
            return slot >= 0 && generation > 0;
        }
    };

}

#endif