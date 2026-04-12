#include "game/systems/DamageSystem.h"

#include <unordered_set>

#include "core/Application.h"
#include "ecs/World.h"
#include "ecs/components/TagComponent.h"
#include "game/audio/AudioBus.h"
#include "game/components/EnemyComponent.h"
#include "game/components/HealthComponent.h"
#include "game/components/ProjectileComponent.h"
#include "game/components/TeamComponent.h"
#include "game/state/ArenaGameState.h"

namespace fw {

namespace {
void ApplyDamage(World& world, Entity target, int damage, std::unordered_set<Entity>& destroyed, ArenaGameState* state) {
    HealthComponent* health = world.GetComponent<HealthComponent>(target);
    if (!health || health->current <= 0) {
        return;
    }

    health->current -= damage;
    if (health->current > 0) {
        if (state && target == world.FindByTag("player")) {
            state->damageFlashTimer = 0.20f;
            state->statusText = TextFormat("Player hit for %d", damage);
            state->statusTextTimer = 0.8f;
            AudioBus::PlayHit();
        }
        return;
    }

    health->current = 0;
    const bool isEnemy = world.HasComponent<EnemyComponent>(target);
    world.DestroyEntity(target);
    destroyed.insert(target);

    if (isEnemy && state) {
        ++state->kills;
        state->score += 100;
        if (state->enemiesRemaining > 0) {
            --state->enemiesRemaining;
        }
        state->statusText = "Enemy down";
        state->statusTextTimer = 0.7f;
        AudioBus::PlayEnemyDown();
    }
}
}

void DamageSystem::Update(Application& app, World& world, float deltaTime) {
    (void)app;
    (void)deltaTime;

    std::unordered_set<Entity> destroyedThisFrame;
    for (const CollisionInfo& collision : world.Collisions()) {
        Entity a = collision.a;
        Entity b = collision.b;

        if (destroyedThisFrame.contains(a) || destroyedThisFrame.contains(b)) {
            continue;
        }

        ProjectileComponent* aProjectile = world.GetComponent<ProjectileComponent>(a);
        ProjectileComponent* bProjectile = world.GetComponent<ProjectileComponent>(b);
        TeamComponent* aTeam = world.GetComponent<TeamComponent>(a);
        TeamComponent* bTeam = world.GetComponent<TeamComponent>(b);

        if (aProjectile && world.HasComponent<HealthComponent>(b) && !world.HasComponent<ProjectileComponent>(b)) {
            if (!(aTeam && bTeam && aTeam->id == bTeam->id)) {
                ApplyDamage(world, b, aProjectile->damage, destroyedThisFrame, m_state);
                if (world.IsAlive(a)) {
                    world.DestroyEntity(a);
                    destroyedThisFrame.insert(a);
                }
            }
            continue;
        }

        if (bProjectile && world.HasComponent<HealthComponent>(a) && !world.HasComponent<ProjectileComponent>(a)) {
            if (!(aTeam && bTeam && aTeam->id == bTeam->id)) {
                ApplyDamage(world, a, bProjectile->damage, destroyedThisFrame, m_state);
                if (world.IsAlive(b)) {
                    world.DestroyEntity(b);
                    destroyedThisFrame.insert(b);
                }
            }
            continue;
        }

        EnemyComponent* aEnemy = world.GetComponent<EnemyComponent>(a);
        EnemyComponent* bEnemy = world.GetComponent<EnemyComponent>(b);

        if (aEnemy && world.FindByTag("player") == b) {
            if (aEnemy->contactDamageCooldownRemaining <= 0.0f) {
                ApplyDamage(world, b, aEnemy->contactDamage, destroyedThisFrame, m_state);
                if (world.IsAlive(a)) {
                    aEnemy->contactDamageCooldownRemaining = aEnemy->contactDamageCooldown;
                }
            }
        } else if (bEnemy && world.FindByTag("player") == a) {
            if (bEnemy->contactDamageCooldownRemaining <= 0.0f) {
                ApplyDamage(world, a, bEnemy->contactDamage, destroyedThisFrame, m_state);
                if (world.IsAlive(b)) {
                    bEnemy->contactDamageCooldownRemaining = bEnemy->contactDamageCooldown;
                }
            }
        }
    }
}

} // namespace fw
