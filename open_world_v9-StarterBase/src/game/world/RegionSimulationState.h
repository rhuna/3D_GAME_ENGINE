#pragma once
#include <string>
#include <vector>
namespace fw { struct RegionSimulationState { std::string regionId; bool safeZone=false; int ambientPopulation=0; int activeEncounters=0; std::vector<std::string> activeActivities; }; }
