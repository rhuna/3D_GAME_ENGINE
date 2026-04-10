#pragma once

namespace fw {

class Time {
public:
    void BeginFrame();

    [[nodiscard]] float DeltaTime() const { return m_deltaTime; }
    [[nodiscard]] float UnscaledDeltaTime() const { return m_unscaledDeltaTime; }
    [[nodiscard]] double TotalTime() const { return m_totalTime; }
    [[nodiscard]] unsigned long long FrameCount() const { return m_frameCount; }

private:
    double m_lastFrameTime = 0.0;
    float m_deltaTime = 0.0f;
    float m_unscaledDeltaTime = 0.0f;
    double m_totalTime = 0.0;
    unsigned long long m_frameCount = 0;
};

} // namespace fw
