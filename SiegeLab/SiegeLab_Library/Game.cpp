#include "pch.h"
#include "framework.h"

#include "Game.h"
#include "HeavyProjectile.h"
#include "InputController.h"
#include "Launcher.h"
#include "SceneObjects.h"
#include "StandardProjectile.h"
#include "raylib.h"

namespace {

    void stepGroundContacts(
        siegelablib::SceneObjects& scene,
        float dt,
        cyclone::ContactResolver& resolver)
    {
        cyclone::CollisionPlane ground;
        ground.direction = cyclone::Vector3(0.0f, 1.0f, 0.0f);
        ground.offset = 0.0f;

        const unsigned maxContacts = 64;
        cyclone::Contact contacts[maxContacts];

        cyclone::CollisionData data;
        data.contactArray = contacts;
        data.contacts = contacts;
        data.contactsLeft = static_cast<int>(maxContacts);
        data.contactCount = 0;
        data.friction = 0.6f;
        data.restitution = 0.15f;
        data.tolerance = 0.01f;

        for (int i = 0; i < siegelablib::SceneObjects::MaxEntities; ++i)
        {
            siegelablib::EntityId id;
            id.slot = i;
            id.generation = 1;

            siegelablib::GameEntity* entity = scene.get(id);
            if (entity == nullptr)
            {
                continue;
            }

            siegelablib::Projectile* proj = dynamic_cast<siegelablib::Projectile*>(entity);
            if (proj != nullptr)
            {
                proj->getSphere().calculateInternals();
                cyclone::CollisionDetector::sphereAndTruePlane(
                    proj->getSphere(), ground, &data);
            }
        }

        if (data.contactCount > 0)
        {
            resolver.resolveContacts(data.contactArray, data.contactCount, dt);
        }
    }

    void drawProjectile(const siegelablib::Projectile& proj, Color color)
    {
        const cyclone::Vector3 pos = proj.getBody().getPosition();
        const ::Vector3 drawPos = {
            static_cast<float>(pos.x),
            static_cast<float>(pos.y),
            static_cast<float>(pos.z)
        };
        DrawSphere(drawPos, proj.getRadius(), color);
    }

    void drawAllProjectiles(siegelablib::SceneObjects& scene)
    {
        for (int i = 0; i < siegelablib::SceneObjects::MaxEntities; ++i)
        {
            siegelablib::EntityId id;
            id.slot = i;
            id.generation = 1;

            siegelablib::GameEntity* entity = scene.get(id);
            if (entity == nullptr)
            {
                continue;
            }

            siegelablib::Projectile* proj = dynamic_cast<siegelablib::Projectile*>(entity);
            if (proj != nullptr)
            {
                if (proj->getRadius() > 0.30f)
                {
                    drawProjectile(*proj, MAROON);
                }
                else
                {
                    drawProjectile(*proj, ORANGE);
                }
            }
        }
    }

    void drawAimLine(const siegelablib::Launcher& launcher, const ::Vector3& origin)
    {
        const siegelablib::AimVec3 dir = launcher.getDirection();
        const ::Vector3 endPos = {
            origin.x + dir.x * 3.5f,
            origin.y + dir.y * 3.5f,
            origin.z + dir.z * 3.5f
        };
        DrawSphere(origin, 0.22f, RED);
        DrawLine3D(origin, endPos, BLUE);
    }

}

void Game::Run()
{
    InitWindow(1000, 650, "SiegeLab - Launcher & Projectile");
    SetTargetFPS(60);

    siegelablib::Launcher launcher;
    siegelablib::InputController input;
    siegelablib::SceneObjects scene;
    cyclone::ContactResolver resolver(16, 16, 0.01f, 0.01f);

    float angleX = 0.0f;
    float angleY = 30.0f;
    float speed = 16.0f;
    bool spaceWasDown = false;

    Camera3D camera = { 0 };
    camera.position = { -12.0f, 6.0f, 10.0f };
    camera.target = { 4.0f, 1.0f, 0.0f };
    camera.up = { 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    const ::Vector3 launchOrigin = { 0.0f, 2.0f, 0.0f };

    while (!WindowShouldClose())
    {
        const float dt = GetFrameTime();

        if (IsKeyPressed(KEY_ONE))
        {
            input.onKeyPressed(1, launcher);
        }
        if (IsKeyPressed(KEY_TWO))
        {
            input.onKeyPressed(2, launcher);
        }

        if (IsKeyDown(KEY_LEFT)) { angleX -= 30.0f * dt; }
        if (IsKeyDown(KEY_RIGHT)) { angleX += 30.0f * dt; }
        if (IsKeyDown(KEY_UP)) { angleY += 30.0f * dt; }
        if (IsKeyDown(KEY_DOWN)) { angleY -= 30.0f * dt; }
        if (IsKeyDown(KEY_W)) { speed += 10.0f * dt; }
        if (IsKeyDown(KEY_S)) { speed -= 10.0f * dt; }

        input.onAimInput(angleX, angleY, speed, launcher);
        angleX = launcher.getAngleXDeg();
        angleY = launcher.getAngleYDeg();
        speed = launcher.getSpeed();

        const bool spaceDown = IsKeyDown(KEY_SPACE);
        if (spaceDown && !spaceWasDown)
        {
            const siegelablib::ShotSettings settings = launcher.makeShotSettings();
            const siegelablib::AimVec3 dir = launcher.getDirection();

            siegelablib::AimVec3 startPos;
            startPos.x = launchOrigin.x;
            startPos.y = launchOrigin.y;
            startPos.z = launchOrigin.z;

            if (settings.kind == siegelablib::ShotKind::Heavy)
            {
                siegelablib::EntityId id = scene.createEntity<siegelablib::HeavyProjectile>();
                siegelablib::Projectile* proj = dynamic_cast<siegelablib::Projectile*>(scene.get(id));
                if (proj != nullptr)
                {
                    proj->fire(startPos, dir, settings.speed);
                }
            }
            else
            {
                siegelablib::EntityId id = scene.createEntity<siegelablib::StandardProjectile>();
                siegelablib::Projectile* proj = dynamic_cast<siegelablib::Projectile*>(scene.get(id));
                if (proj != nullptr)
                {
                    proj->fire(startPos, dir, settings.speed);
                }
            }
        }
        spaceWasDown = spaceDown;

        scene.updateAll(dt);
        stepGroundContacts(scene, dt, resolver);
        scene.flushRemovals();

        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        DrawGrid(20, 1.0f);
        drawAimLine(launcher, launchOrigin);
        drawAllProjectiles(scene);
        EndMode3D();

        const char* kindLabel = (launcher.getKind() == siegelablib::ShotKind::Heavy)
            ? "Projectile: Heavy (2 kg, r=0.36)"
            : "Projectile: Standard (1 kg, r=0.28)";

        DrawText(kindLabel, 20, 20, 20, BLACK);
        DrawText(TextFormat("Angle X: %.1f | Angle Y: %.1f | Vitesse: %.1f",
            launcher.getAngleXDeg(), launcher.getAngleYDeg(), launcher.getSpeed()),
            20, 48, 18, DARKGRAY);
        DrawText("Fleches: viser | W/S: vitesse | 1/2: type | Espace: tirer",
            20, 74, 18, GRAY);

        EndDrawing();
    }

    CloseWindow();
}