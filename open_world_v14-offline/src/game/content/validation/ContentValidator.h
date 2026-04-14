#pragma once
#include <string>
#include <vector>
#include "game/content/ContentPipeline.h"
namespace fw { struct ContentValidationMessage { std::string severity; std::string text; }; class ContentValidator { public: std::vector<ContentValidationMessage> Validate(const ContentPipeline& p) const; }; }
