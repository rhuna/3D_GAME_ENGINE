#include "game/ui/DialogueBox.h"
#include "raylib.h"
namespace fw { void DrawDialogueBox(const DialogueBoxState& state){ if(!state.active) return; DrawRectangle(40,680,1520,180,Fade(BLACK,0.78f)); DrawRectangleLines(40,680,1520,180,WHITE); DrawText(state.speaker.c_str(),70,705,28,YELLOW); DrawText(state.text.c_str(),70,745,24,RAYWHITE); DrawText("Enter / E to continue",70,810,20,GRAY); } }
