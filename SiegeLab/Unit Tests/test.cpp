#include "pch.h"
#include "../SiegeLab_Library/EntityId.h"
#include "../SiegeLab_Library/ObjectPool.h"
#include "../SiegeLab_Library/SceneObjects.h"

namespace {
    class DummyProjectileA : public siegelablib::GameEntity {
    public:
        void draw() override {}
    };

    class DummyProjectileB : public siegelablib::GameEntity {
    public:
        void draw() override {}
    };
}

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

TEST(PoolTests, P02_RecycledSlotChangesGeneration) {
    siegelablib::ObjectPool<int, 1> pool;

    siegelablib::EntityId idA = pool.create(7);
    int genAvant = idA.generation;

    EXPECT_TRUE(pool.destroy(idA));

    siegelablib::EntityId idB = pool.create(42);
    EXPECT_EQ(idB.slot, 0);
    EXPECT_NE(idB.generation, genAvant);
}

TEST(PoolTests, P03_StaleHandleRejected) {
    siegelablib::ObjectPool<int, 1> pool;

    siegelablib::EntityId idA = pool.create(7);
    EXPECT_TRUE(pool.destroy(idA));

    siegelablib ::EntityId idB = pool.create(42);

    EXPECT_EQ(pool.get(idA), nullptr);

    ASSERT_NE(pool.get(idB), nullptr);
    EXPECT_EQ(*pool.get(idB), 42);
}

TEST(PoolTests, P04_LiveAddressStable) {
    siegelablib::ObjectPool<int, 3> pool;

    siegelablib::EntityId idA = pool.create(10);
    int* adresseA = pool.get(idA);
    ASSERT_NE(adresseA, nullptr);

    pool.create(20);
    pool.create(30);

    int* adresseApres = pool.get(idA);
    EXPECT_EQ(adresseApres, adresseA);
    ASSERT_NE(adresseApres, nullptr);
    EXPECT_EQ(*adresseApres, 10);
}

TEST(PoolTests, P05_ClearInvalidatesHandles) {
    siegelablib::ObjectPool<int, 2> pool;

    siegelablib::EntityId idA = pool.create(10);
    siegelablib::EntityId idB = pool.create(20);

    pool.clear();

    EXPECT_EQ(pool.get(idA), nullptr);
    EXPECT_EQ(pool.get(idB), nullptr);

    siegelablib::EntityId idC = pool.create(30);
    EXPECT_GE(idC.slot, 0);
    ASSERT_NE(pool.get(idC), nullptr);
    EXPECT_EQ(*pool.get(idC), 30);
}

TEST(SceneObjectsTests, GO01_FactoryCreatesDerivedTypes) {
    siegelablib::SceneObjects scene;

    siegelablib::EntityId id1 = scene.createEntity<DummyProjectileA>();
    siegelablib::EntityId id2 = scene.createEntity<DummyProjectileB>();

    EXPECT_GE(id1.slot, 0);
    EXPECT_GE(id2.slot, 0);
    EXPECT_NE(id1.slot, id2.slot);

    siegelablib::GameEntity* entite1 = scene.get(id1);
    siegelablib::GameEntity* entite2 = scene.get(id2);

    EXPECT_NE(entite1, nullptr);
    EXPECT_NE(entite2, nullptr);
    EXPECT_NE(entite1, entite2);
}

TEST(SceneObjectsTests, GO05_StableAddressAndSafeRemoval) {
    siegelablib::SceneObjects scene;

    siegelablib::EntityId idA = scene.createEntity<DummyProjectileA>();
    siegelablib::GameEntity* adresseAvant = scene.get(idA);
    ASSERT_NE(adresseAvant, nullptr);

    scene.createEntity<DummyProjectileB>();
    scene.createEntity<DummyProjectileA>();
    scene.createEntity<DummyProjectileB>();

    EXPECT_EQ(scene.get(idA), adresseAvant);

    scene.markForRemoval(idA);
    scene.flushRemovals();

    EXPECT_EQ(scene.get(idA), nullptr);
}

TEST(SceneObjectsTests, GO06_ConstrainedFactoryChecksType) {
    EXPECT_TRUE((std::is_base_of<siegelablib::GameEntity, DummyProjectileA>::value));

    EXPECT_FALSE((std::is_base_of<siegelablib::GameEntity, int>::value));

    siegelablib::SceneObjects scene;
    siegelablib::EntityId id = scene.createEntity<DummyProjectileA>();
    EXPECT_NE(scene.get(id), nullptr);
}

TEST(BlockTest , C001)
{
    
    
}