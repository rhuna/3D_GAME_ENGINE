#pragma once

#include <string>
#include <vector>

namespace fw {

struct StreamingDebugLine {
    std::string label;
    std::string value;
};

class StreamingDebugOverlay {
public:
    void Clear();
    void AddLine(const std::string& label, const std::string& value);
    const std::vector<StreamingDebugLine>& Lines() const;

private:
    std::vector<StreamingDebugLine> m_lines;
};

} // namespace fw
