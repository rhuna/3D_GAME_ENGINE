#include "game/systems/SpawnPersistenceSystem.h"

#include "core/Application.h"

namespace fw {

void SpawnPersistenceSystem::Update(Application&, World&, float) {
    (void)m_state;
}

} // namespace fw
