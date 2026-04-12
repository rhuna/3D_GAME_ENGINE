#include "game/interaction/InteractionSystem.h"

#include <limits>

#include "raylib.h"
#include "raymath.h"

#include "core/Application.h"
#include "core/Input.h"
#include "ecs/World.h"
#include "ecs/components/TagComponent.h"
#include "ecs/components/TransformComponent.h"
#include "game/interaction/InteractableComponent.h"
#include "game/inventory/InventoryComponent.h"
#include "game/npc/NpcComponent.h"
#include "game/world/TravelTriggerComponent.h"

namespace fw {

Entity InteractionSystem::FindBestInteractable(const World& world, Entity player) const {
    const TransformComponent* playerTransform = world.GetComponent<TransformComponent>(player);
    if (!playerTransform) return 0;

    Entity best = 0;
    float bestDistance = std::numeric_limits<float>::max();
    for (Entity entity : world.Entities()) {
        const InteractableComponent* interactable = world.GetComponent<InteractableComponent>(entity);
        const TransformComponent* transform = world.GetComponent<TransformComponent>(entity);
        if (!interactable || !interactable->enabled || !transform) continue;
        const float distance = Vector3Distance(playerTransform->position, transform->position);
        if (distance <= interactable->radius && distance < bestDistance) {
            best = entity;
            bestDistance = distance;
        }
    }
    return best;
}

void InteractionSystem::Update(Application& app, World& world, float) {
    if (!m_state) return;
    m_state->hoveredPrompt.clear();

    const Entity player = world.FindByTag("player");
    if (player == 0) return;

    Entity target = FindBestInteractable(world, player);
    if (target == 0) return;

    const InteractableComponent* interactable = world.GetComponent<InteractableComponent>(target);
    if (!interactable) return;
    m_state->hoveredPrompt = interactable->prompt;

    if (!app.GetInput().IsKeyPressed(KEY_E)) {
        return;
    }

    m_state->lastInteraction = interactable->prompt;
    switch (interactable->kind) {
        case InteractionKind::Talk: {
            if (const NpcComponent* npc = world.GetComponent<NpcComponent>(target)) {
                m_state->dialogue.Begin(npc->displayName, npc->dialogueLines);
            }
            break;
        }
        case InteractionKind::Loot: {
            InventoryComponent* inventory = world.GetComponent<InventoryComponent>(player);
            const TagComponent* tag = world.GetComponent<TagComponent>(target);
            if (inventory && tag) {
                inventory->items.push_back({tag->value});
                world.DestroyEntity(target);
            }
            break;
        }
        case InteractionKind::SavePoint: {
            m_state->saveProfile.currentRegion = m_state->saveProfile.currentRegion.empty() ? "village" : m_state->saveProfile.currentRegion;
            m_state->saveProfile.SaveToFile("assets/saves/open_world_profile.txt");
            break;
        }
        case InteractionKind::Travel:
        case InteractionKind::Inspect:
        default:
            break;
    }
}

} // namespace fw
