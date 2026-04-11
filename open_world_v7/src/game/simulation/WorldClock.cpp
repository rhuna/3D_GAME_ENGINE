#include "game/simulation/WorldClock.h"
#include <cstdio>
namespace fw { void WorldClock::Reset(float startHour){ m_hour=startHour; m_day=1; } void WorldClock::Update(float dtSeconds){ m_hour += dtSeconds * m_timeScale; while(m_hour>=24.0f){ m_hour -= 24.0f; ++m_day; } } std::string WorldClock::GetFormattedTime() const { int h=(int)m_hour; int m=(int)((m_hour-(float)h)*60.0f); char b[32]; std::snprintf(b,sizeof(b),"Day %d %02d:%02d",m_day,h,m); return std::string(b);} }
