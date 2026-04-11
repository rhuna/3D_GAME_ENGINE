#pragma once
#include <string>
#include <unordered_map>
namespace fw { class FactionSystem { public: void Reset(); void AddReputation(const std::string& factionId, int delta); int GetReputation(const std::string& factionId) const; std::string DescribeStanding(const std::string& factionId) const; private: std::unordered_map<std::string, int> m_reputation; }; }
