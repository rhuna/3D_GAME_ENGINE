#include "PrefabLibrary.h"
#include <string>

namespace fw
{
    bool PrefabLibrary::AppendFromDirectory(const std::string& path)
    {
        // TODO: implement real prefab loading from disk (JSON/YAML)
        (void)path;
        return true;
    }

    bool PrefabLibrary::AppendVariantsFromDirectory(const std::string& path)
    {
        // TODO: implement real variant loading from disk
        (void)path;
        return true;
    }
}