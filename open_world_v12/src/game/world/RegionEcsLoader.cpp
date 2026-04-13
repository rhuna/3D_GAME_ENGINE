#include "game/world/RegionEcsLoader.h"

#include <string>

#include "core/Application.h"
#include "core/Logger.h"
#include "ecs/World.h"
#include "ecs/components/EditorMetadataComponent.h"
#include "ecs/components/RenderComponent.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "game/content/ContentPipeline.h"
#include "game/content/models/RegionDefinition.h"
#include "game/save/SaveGameProfile.h"
#include "game/components/GatherNodeComponent.h"
#include "game/components/PersistentIdComponent.h"
#include "game/components/SavePointComponent.h"
#include "game/npc/NpcComponent.h"
#include "game/world/RegionLayoutDatabase.h"
#include "game/world/TravelTriggerComponent.h"
#include "gameplay/prefabs/PrefabDefinition.h"
#include "gameplay/prefabs/SpawnFactory.h"
#include "scene/data/SceneDefinition.h"
#include "scene/spawning/SceneSpawner.h"

namespace fw {
namespace {

std::string MakePersistentId(const std::string& regionId, const std::string& kind, std::size_t index) {
    return regionId + ":" + kind + ":" + std::to_string(index);
}

std::string ToVec3String(const Vector3& v) {
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ")";
}

void SpawnLayoutProp(World& world, const LayoutPrimitive& primitive, std::size_t index) {
    PrefabDefinition prefab;
    prefab.name = "runtime_layout_prop_" + std::to_string(index);
    prefab.tag.value = primitive.type == LayoutPrimitiveType::Cylinder ? "region_prop_cylinder" : "region_prop_box";
    prefab.transform.position = primitive.position;
    prefab.transform.scale = primitive.size;
    prefab.render.tint = primitive.color;
    prefab.render.drawCube = primitive.type == LayoutPrimitiveType::Box;
    prefab.render.drawSphere = false;
    prefab.hasCollider = true;
    prefab.collider.isStatic = true;
    prefab.collider.halfExtents = Vector3{primitive.size.x * 0.5f, primitive.size.y * 0.5f, primitive.size.z * 0.5f};

    const Entity entity = spawn::SpawnFromPrefab(world, prefab);
    if (primitive.type == LayoutPrimitiveType::Cylinder && entity != 0) {
        if (RenderComponent* render = world.GetComponent<RenderComponent>(entity)) {
            render->drawCube = false;
            render->drawSphere = true;
            render->sphereRadius = primitive.size.x;
        }
    }

    Logger::Debug(
        "SCENE",
        "Spawned layout prop index=" + std::to_string(index) +
        " entity=" + std::to_string(static_cast<unsigned int>(entity)) +
        " at " + ToVec3String(primitive.position)
    );
}

void SpawnMarker(World& world, const std::string& tag, const Vector3& position, const Vector3& scale, Color tint, const std::string& source) {
    const Entity entity = spawn::SpawnStaticBox(world, tag, position, scale, tint);
    if (entity != 0) {
        if (EditorMetadataComponent* meta = world.GetComponent<EditorMetadataComponent>(entity)) {
            meta->sourcePrefab = source;
        }
    }

    Logger::Info(
        "SCENE",
        "Spawned marker tag=" + tag +
        " entity=" + std::to_string(static_cast<unsigned int>(entity)) +
        " at " + ToVec3String(position)
    );
}

} // namespace

void RegionEcsLoader::Rebuild(Application& app,
                              const ContentPipeline& pipeline,
                              const RegionLayoutDatabase& layouts,
                              const SaveGameProfile& profile,
                              const std::string& regionId) const {
    World& world = app.GetWorld();
    Logger::Info("REGION", "Rebuilding region ECS for region: " + regionId);
    world.Clear();

    const RegionLayout* layout = layouts.Find(regionId);
    if (!layout) {
        Logger::Warn("REGION", "RegionEcsLoader could not find layout for region: " + regionId);
        return;
    }

    Logger::Info(
        "REGION",
        "Layout counts props=" + std::to_string(layout->props.size()) +
        " gatherPoints=" + std::to_string(layout->gatherPoints.size()) +
        " npcSpawns=" + std::to_string(layout->npcSpawns.size()) +
        " travelPoints=" + std::to_string(layout->travelPoints.size())
    );

    std::size_t propIndex = 0;
    for (const LayoutPrimitive& primitive : layout->props) {
        SpawnLayoutProp(world, primitive, propIndex++);
    }

    SpawnMarker(world, "player_start", profile.playerPosition, Vector3{0.8f, 1.8f, 0.8f}, Color{220, 70, 70, 255}, "runtime_player_start");

    for (std::size_t i = 0; i < layout->gatherPoints.size(); ++i) {
        const Vector3 markerPos{layout->gatherPoints[i].x, layout->gatherPoints[i].y + 0.45f, layout->gatherPoints[i].z};
        const Entity e = spawn::SpawnStaticBox(world,
                    "gather_marker",
                    markerPos,
                    Vector3{0.6f, 0.6f, 0.6f},
                    GOLD);
        if (e != 0) {
            GatherNodeComponent gather{};
            gather.nodeId = MakePersistentId(regionId, "gather_node", i);
            gather.resourceId = "herb";
            gather.remainingUses = 1;
            gather.respawnSeconds = 300.0f;
            gather.harvested = false;
            world.AddComponent<GatherNodeComponent>(e, gather);
            world.AddComponent<PersistentIdComponent>(e, PersistentIdComponent{MakePersistentId(regionId, "gather", i)});
            Logger::Info(
                "SPAWN",
                "Spawned gather node id=" + gather.nodeId +
                " entity=" + std::to_string(static_cast<unsigned int>(e)) +
                " at " + ToVec3String(markerPos)
            );
        }
    }

    for (std::size_t i = 0; i < layout->npcSpawns.size(); ++i) {
        const Vector3 markerPos{layout->npcSpawns[i].x, layout->npcSpawns[i].y + 1.0f, layout->npcSpawns[i].z};
        const Entity e = spawn::SpawnStaticBox(world,
                    "npc_spawn_marker",
                    markerPos,
                    Vector3{0.9f, 2.0f, 0.9f},
                    PURPLE);
        if (e != 0) {
            NpcComponent npc{};
            npc.npcId = "runtime_npc_" + std::to_string(i);
            npc.displayName = "NPC Spawn";
            world.AddComponent<NpcComponent>(e, npc);
            world.AddComponent<PersistentIdComponent>(e, PersistentIdComponent{MakePersistentId(regionId, "npc", i)});
            Logger::Info(
                "SPAWN",
                "Spawned NPC marker npcId=" + npc.npcId +
                " entity=" + std::to_string(static_cast<unsigned int>(e)) +
                " at " + ToVec3String(markerPos)
            );
        }
    }

    for (std::size_t i = 0; i < layout->travelPoints.size(); ++i) {
        const Vector3 markerPos{layout->travelPoints[i].x, layout->travelPoints[i].y + 1.0f, layout->travelPoints[i].z};
        const Entity travelEntity = spawn::SpawnStaticBox(world,
                                                          "travel_marker",
                                                          markerPos,
                                                          Vector3{1.3f, 2.0f, 1.3f},
                                                          SKYBLUE);
        if (travelEntity != 0) {
            TravelTriggerComponent trigger;
            if (regionId == "village_region") trigger.destinationRegion = "forest_region";
            else if (regionId == "forest_region") trigger.destinationRegion = (i == 0 ? "village_region" : "ruins_region");
            else trigger.destinationRegion = "village_region";
            trigger.destinationSpawn = Vector3{0.0f, 1.0f, 0.0f};
            world.AddComponent<TravelTriggerComponent>(travelEntity, trigger);
            world.AddComponent<PersistentIdComponent>(travelEntity, PersistentIdComponent{MakePersistentId(regionId, "travel", i)});
            Logger::Info(
                "SPAWN",
                "Spawned travel marker entity=" + std::to_string(static_cast<unsigned int>(travelEntity)) +
                " destination=" + trigger.destinationRegion +
                " at " + ToVec3String(markerPos)
            );
        }
    }

    if (const RegionDefinition* region = pipeline.regions.Find(regionId)) {
        std::string sceneKey = region->sceneFile;
        Logger::Info("REGION", "Region definition scene file: " + region->sceneFile);
        const std::size_t dot = sceneKey.rfind('.');
        if (dot != std::string::npos) {
            sceneKey = sceneKey.substr(0, dot);
        }
        Logger::Info("SCENE", "Looking up scene key: " + sceneKey);
        if (const SceneDefinition* scene = app.GetSceneLibrary().Find(sceneKey)) {
            Logger::Info("SCENE", "Scene definition found: " + sceneKey);
            SceneSpawner::Spawn(world, app.GetPrefabLibrary(), *scene);
            Logger::Info("SCENE", "Spawned scene content for region: " + regionId + " using scene key: " + sceneKey);
        } else {
            Logger::Warn("SCENE", "Scene definition not found for key: " + sceneKey);
        }
    } else {
        Logger::Warn("REGION", "Region definition not found in pipeline for region: " + regionId);
    }

    std::size_t saveIndex = 0;
    std::size_t taggedTravelIndex = 0;
    std::size_t taggedGatherIndex = 0;
    for (const Entity entity : world.Entities()) {
        const TagComponent* tag = world.GetComponent<TagComponent>(entity);
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        if (!tag || !transform) continue;

        Logger::Debug(
            "SCENE",
            "Entity tag=" + tag->value +
            " entity=" + std::to_string(static_cast<unsigned int>(entity)) +
            " at " + ToVec3String(transform->position)
        );

        if (tag->value == "save_point") {
            if (!world.HasComponent<SavePointComponent>(entity)) {
                world.AddComponent<SavePointComponent>(entity, SavePointComponent{});
                Logger::Info("SAVE", "Attached SavePointComponent to entity=" + std::to_string(static_cast<unsigned int>(entity)));
            }
            if (!world.HasComponent<PersistentIdComponent>(entity)) {
                const std::string id = MakePersistentId(regionId, "save", saveIndex++);
                world.AddComponent<PersistentIdComponent>(entity, PersistentIdComponent{id});
                Logger::Info("SAVE", "Assigned persistent id=" + id + " to save point entity=" + std::to_string(static_cast<unsigned int>(entity)));
            }
        } else if (tag->value == "region_exit" || tag->value == "travel_marker") {
            if (!world.HasComponent<TravelTriggerComponent>(entity)) {
                TravelTriggerComponent trigger;
                if (regionId == "village_region") trigger.destinationRegion = "forest_region";
                else if (regionId == "forest_region") trigger.destinationRegion = (transform->position.x < 0.0f ? "village_region" : "ruins_region");
                else trigger.destinationRegion = "village_region";
                trigger.destinationSpawn = Vector3{0.0f, 1.0f, 0.0f};
                world.AddComponent<TravelTriggerComponent>(entity, trigger);
                Logger::Info(
                    "REGION",
                    "Attached travel trigger to entity=" + std::to_string(static_cast<unsigned int>(entity)) +
                    " destination=" + trigger.destinationRegion
                );
            }
            if (!world.HasComponent<PersistentIdComponent>(entity)) {
                const std::string id = MakePersistentId(regionId, "travel_scene", taggedTravelIndex++);
                world.AddComponent<PersistentIdComponent>(entity, PersistentIdComponent{id});
                Logger::Info("REGION", "Assigned persistent id=" + id + " to travel entity=" + std::to_string(static_cast<unsigned int>(entity)));
            }
        } else if (tag->value == "gather_marker") {
            if (!world.HasComponent<GatherNodeComponent>(entity)) {
                GatherNodeComponent gather{};
                gather.nodeId = MakePersistentId(regionId, "gather_scene_node", taggedGatherIndex);
                gather.resourceId = "herb";
                gather.remainingUses = 1;
                gather.respawnSeconds = 300.0f;
                gather.harvested = false;
                world.AddComponent<GatherNodeComponent>(entity, gather);
                Logger::Info(
                    "SPAWN",
                    "Attached gather component nodeId=" + gather.nodeId +
                    " to tagged scene entity=" + std::to_string(static_cast<unsigned int>(entity))
                );
            }
            if (!world.HasComponent<PersistentIdComponent>(entity)) {
                const std::string id = MakePersistentId(regionId, "gather_scene", taggedGatherIndex++);
                world.AddComponent<PersistentIdComponent>(entity, PersistentIdComponent{id});
                Logger::Info("SPAWN", "Assigned persistent id=" + id + " to gather entity=" + std::to_string(static_cast<unsigned int>(entity)));
            }
        }
    }

    Logger::Info("REGION", "Finished region rebuild for: " + regionId + ". Entity count=" + std::to_string(world.Entities().size()));
}

} // namespace fw
