#include "game/tooling/StreamingDebugOverlay.h"

namespace fw {

void StreamingDebugOverlay::Clear() {
    m_lines.clear();
}

void StreamingDebugOverlay::AddLine(const std::string& label, const std::string& value) {
    m_lines.push_back({label, value});
}

const std::vector<StreamingDebugLine>& StreamingDebugOverlay::Lines() const {
    return m_lines;
}

} // namespace fw
