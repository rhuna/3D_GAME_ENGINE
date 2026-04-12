#include "core/Time.h"

#include "raylib.h"

namespace fw {

void Time::BeginFrame() {
    const double now = GetTime();

    if (m_frameCount == 0) {
        m_lastFrameTime = now;
    }

    m_unscaledDeltaTime = static_cast<float>(now - m_lastFrameTime);
    m_deltaTime = m_unscaledDeltaTime;
    m_totalTime += m_unscaledDeltaTime;
    m_lastFrameTime = now;
    ++m_frameCount;
}

} // namespace fw
