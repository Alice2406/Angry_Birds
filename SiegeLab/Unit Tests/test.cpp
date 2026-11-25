#include "pch.h"
#include <Game.h>
#include "../SiegeLab/EntityId.h"
#include "../SiegeLab/ObjectPool.h"

TEST(PoolTests, P01_CapacityLimit) {
    siegelablib::ObjectPool<int, 3> pool;

    siegelablib::EntityId id1 = pool.create(10);
    siegelablib::EntityId id2 = pool.create(20);
    siegelablib::EntityId id3 = pool.create(30);

    siegelablib::EntityId id4 = pool.create(40);

    EXPECT_GE(id1.slot, 0);
    EXPECT_GE(id2.slot, 0);
    EXPECT_GE(id3.slot, 0);
    EXPECT_EQ(id4.slot, -1);

    ASSERT_NE(pool.get(id1), nullptr);
    ASSERT_NE(pool.get(id2), nullptr);
    ASSERT_NE(pool.get(id3), nullptr);

    EXPECT_EQ(*pool.get(id1), 10);
    EXPECT_EQ(*pool.get(id2), 20);
    EXPECT_EQ(*pool.get(id3), 30);
}

TEST(PoolTests, P02_RecycledSlotChangesGeneration)
{
    siegelablib::ObjectPool<int, 1> pool;
    
}