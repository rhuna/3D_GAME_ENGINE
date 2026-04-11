#pragma once
#include <string>
namespace fw { class WorldClock { public: void Reset(float startHour = 8.0f); void Update(float dtSeconds); void SetTimeScale(float scale) { m_timeScale = scale; } float GetHour() const { return m_hour; } int GetDay() const { return m_day; } std::string GetFormattedTime() const; private: float m_hour = 8.0f; int m_day = 1; float m_timeScale = 0.20f; }; }
