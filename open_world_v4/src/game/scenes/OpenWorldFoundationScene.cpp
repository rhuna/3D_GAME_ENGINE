#include "game/scenes/OpenWorldFoundationScene.h"

#include <memory>

#include "raylib.h"
#include "raymath.h"

#include "core/Application.h"
#include "core/Input.h"
#include "ecs/World.h"
#include "ecs/components/RigidbodyComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "ecs/systems/CollisionSystem.h"
#include "ecs/systems/MovementSystem.h"
#include "game/components/HealthComponent.h"
#include "game/components/PlayerComponent.h"
#include "game/components/TeamComponent.h"
#include "game/interaction/InteractableComponent.h"
#include "game/interaction/InteractionSystem.h"
#include "game/inventory/InventoryComponent.h"
#include "game/inventory/InventorySystem.h"
#include "game/npc/NpcComponent.h"
#include "game/save/SaveGameProfile.h"
#include "game/systems/DamageSystem.h"
#include "game/systems/EnemyAISystem.h"
#include "game/systems/PlayerControllerSystem.h"
#include "game/world/TravelTriggerComponent.h"
#include "gameplay/prefabs/PrefabLibrary.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "gameplay/registry/SystemRegistry.h"
#include "gameplay/systems/ProjectileCleanupSystem.h"
#include "scene/data/SceneDefinition.h"
#include "scene/spawning/SceneSpawner.h"

namespace fw {

namespace {
constexpr int kPlayerTeam = 1;
}

void OpenWorldFoundationScene::OnEnter(Application& app) {
    m_regionManager.RegisterDefaults();
    m_state.saveProfile.LoadFromFile("assets/saves/open_world_profile.txt");
    if (m_state.saveProfile.quests.empty()) {
        m_state.saveProfile.ResetToDefaults();
    }
    LoadRegion(app, m_state.saveProfile.currentRegion, true);
}

void OpenWorldFoundationScene::OnExit(Application& app) {
    m_state.saveProfile.SaveToFile("assets/saves/open_world_profile.txt");
    app.GetSystemRegistry().Clear();
    app.GetWorld().Clear();
}

void OpenWorldFoundationScene::LoadRegion(Application& app, const std::string& regionId, bool useSavedPosition) {
    auto& world = app.GetWorld();
    auto& systems = app.GetSystemRegistry();
    auto& prefabs = app.GetPrefabLibrary();
    auto& scenes = app.GetSceneLibrary();

    world.Clear();
    systems.Clear();

    systems.RegisterUpdate(std::make_unique<PlayerControllerSystem>());
    systems.RegisterUpdate(std::make_unique<EnemyAISystem>());
    systems.RegisterUpdate(std::make_unique<DamageSystem>(nullptr));
    systems.RegisterUpdate(std::make_unique<InteractionSystem>(&m_state));
    systems.RegisterUpdate(std::make_unique<InventorySystem>(&m_state));
    systems.RegisterFixed(std::make_unique<MovementSystem>());
    systems.RegisterFixed(std::make_unique<CollisionSystem>());
    systems.RegisterFixed(std::make_unique<ProjectileCleanupSystem>());

    m_regionManager.SetCurrent(regionId);
    m_state.saveProfile.currentRegion = regionId;
    const RegionDescriptor* region = m_regionManager.Current();
    m_state.regionBanner = region ? region->displayName : regionId;

    const SceneDefinition* scene = region ? scenes.Find(region->sceneName) : nullptr;
    if (!scene || !SceneSpawner::Spawn(world, prefabs, *scene)) {
        spawn::SpawnStaticBox(world, "ground", Vector3{0.0f, -0.5f, 0.0f}, Vector3{48.0f, 1.0f, 48.0f}, Color{75, 95, 78, 255});
        spawn::SpawnPlayer(world, Vector3{0.0f, 1.5f, 0.0f});
    }

    BootstrapRegionGameplay(world);

    Entity player = world.FindByTag("player");
    if (player != 0) {
        if (TransformComponent* transform = world.GetComponent<TransformComponent>(player)) {
            if (useSavedPosition) {
                transform->position = m_state.saveProfile.playerPosition;
            }
        }
        if (HealthComponent* health = world.GetComponent<HealthComponent>(player)) {
            health->current = m_state.saveProfile.playerHealth;
            health->maximum = m_state.saveProfile.playerMaxHealth;
        }
        if (InventoryComponent* inventory = world.GetComponent<InventoryComponent>(player)) {
            *inventory = m_state.saveProfile.inventory;
        }
    }
}

void OpenWorldFoundationScene::BootstrapRegionGameplay(World& world) {
    const Entity player = world.FindByTag("player");
    if (player != 0) {
        world.AddComponent<PlayerComponent>(player, PlayerComponent{});
        world.AddComponent<HealthComponent>(player, HealthComponent{m_state.saveProfile.playerHealth, m_state.saveProfile.playerMaxHealth});
        world.AddComponent<TeamComponent>(player, TeamComponent{kPlayerTeam});
        world.AddComponent<InventoryComponent>(player, m_state.saveProfile.inventory);
        if (RigidbodyComponent* body = world.GetComponent<RigidbodyComponent>(player)) {
            body->useGravity = true;
            body->drag = 10.0f;
        }
    }

    for (Entity entity : world.Entities()) {
        const TagComponent* tag = world.GetComponent<TagComponent>(entity);
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        if (!tag || !transform) continue;

        if (tag->value == "npc_villager") {
            world.AddComponent<InteractableComponent>(entity, {InteractionKind::Talk, "Press E to talk", 2.5f, true});
            world.AddComponent<NpcComponent>(entity, {"villager_elder", "Village Elder", {"The road ahead leads through Pinewatch Forest.", "Keep your eyes open for old stones and ruined watchtowers.", "When you reach the ruins, your first quest begins for real."}, true, false});
        } else if (tag->value == "region_exit") {
            TravelTriggerComponent trigger {};
            if (transform->position.z < -20.0f) {
                trigger.destinationRegion = "forest";
                trigger.destinationSpawn = Vector3{0.0f, 1.5f, 14.0f};
            } else if (transform->position.x > 30.0f) {
                trigger.destinationRegion = "ruins";
                trigger.destinationSpawn = Vector3{-18.0f, 1.5f, 0.0f};
            } else {
                trigger.destinationRegion = "village";
                trigger.destinationSpawn = Vector3{0.0f, 1.5f, 16.0f};
            }
            world.AddComponent<TravelTriggerComponent>(entity, trigger);
            world.AddComponent<InteractableComponent>(entity, {InteractionKind::Travel, "Press E to travel", 3.0f, true});
        } else if (tag->value == "save_point") {
            world.AddComponent<InteractableComponent>(entity, {InteractionKind::SavePoint, "Press E to save at the waystone", 2.5f, true});
        } else if (tag->value == "item_pickup_herb") {
            world.AddComponent<InteractableComponent>(entity, {InteractionKind::Loot, "Press E to gather herb", 2.0f, true});
        } else if (tag->value == "enemy" && !world.HasComponent<TeamComponent>(entity)) {
            world.AddComponent<TeamComponent>(entity, TeamComponent{2});
            world.AddComponent<HealthComponent>(entity, HealthComponent{40, 40});
        }
    }
}

void OpenWorldFoundationScene::TryTravel(Application& app) {
    auto& world = app.GetWorld();
    const Entity player = world.FindByTag("player");
    const TransformComponent* playerTransform = world.GetComponent<TransformComponent>(player);
    if (!playerTransform) return;

    for (Entity entity : world.Entities()) {
        const TravelTriggerComponent* trigger = world.GetComponent<TravelTriggerComponent>(entity);
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        if (!trigger || !transform) continue;
        const float distance = Vector3Distance(playerTransform->position, transform->position);
        if (distance > trigger->activationRadius) continue;
        if (trigger->requiresInteraction && !app.GetInput().IsKeyPressed(KEY_E)) continue;
        m_state.saveProfile.playerPosition = trigger->destinationSpawn;
        LoadRegion(app, trigger->destinationRegion, true);
        return;
    }
}

void OpenWorldFoundationScene::Update(Application& app, float deltaTime) {
    if (m_state.dialogue.active) {
        if (app.GetInput().IsKeyPressed(KEY_E) || app.GetInput().IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            m_state.dialogue.Advance();
        }
    }

    app.GetSystemRegistry().UpdateAll(app, app.GetWorld(), deltaTime);
    TryTravel(app);

    Entity player = app.GetWorld().FindByTag("player");
    if (player != 0) {
        if (const TransformComponent* transform = app.GetWorld().GetComponent<TransformComponent>(player)) {
            m_state.saveProfile.playerPosition = transform->position;
            Camera3D& camera = app.GetCamera();
            const Vector3 desired = Vector3Add(transform->position, Vector3{0.0f, 7.0f, 11.0f});
            camera.position = Vector3Lerp(camera.position, desired, 0.08f);
            camera.target = Vector3Lerp(camera.target, Vector3Add(transform->position, Vector3{0.0f, 1.3f, 0.0f}), 0.12f);
        }
        if (const HealthComponent* health = app.GetWorld().GetComponent<HealthComponent>(player)) {
            m_state.saveProfile.playerHealth = health->current;
            m_state.saveProfile.playerMaxHealth = health->maximum;
        }
    }

    if (app.GetInput().IsKeyPressed(KEY_F9)) {
        m_state.saveProfile.SaveToFile("assets/saves/open_world_profile.txt");
    }
}

void OpenWorldFoundationScene::FixedUpdate(Application& app, float fixedDeltaTime) {
    app.GetSystemRegistry().FixedUpdateAll(app, app.GetWorld(), fixedDeltaTime);
}

void OpenWorldFoundationScene::Render(Application& app) {
    app.GetRenderer().DrawGrid(80, 1.0f);
    app.GetRenderer().DrawWorld(app.GetWorld(), app.GetAssets());
    app.GetSystemRegistry().RenderAll(app, app.GetWorld());
}

void OpenWorldFoundationScene::DrawUi(Application& app) {
    m_hud.Draw(app.GetWorld(), m_state);
}

} // namespace fw
