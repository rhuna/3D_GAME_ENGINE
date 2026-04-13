#pragma once
#include "game/template/VerticalSliceManifest.h"

namespace fw {

inline VerticalSliceManifest MakeStarterValeSlice() {
    return VerticalSliceManifest{
        "starter_vale_slice",
        "Starter Vale Vertical Slice",
        "assets/scenes/starter_vale.scene",
        "assets/scenes/starter_hut_interior.scene",
        "first_medicine",
        {
            {"talk_mara", "Speak with Mara the herbalist."},
            {"gather_herbs", "Gather herbs from the nearby field."},
            {"craft_medicine", "Craft field medicine at the bench."},
            {"return_to_mara", "Return the medicine to Mara."}
        }
    };
}

} // namespace fw
