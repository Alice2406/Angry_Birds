#pragma once

namespace siegelab {

    class GameEntity {
    public:
        virtual ~GameEntity() = default;

        virtual void draw() = 0;
    };

}