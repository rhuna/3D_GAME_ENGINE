#include "game/ui/QuestLogPanel.h"
#include "game/quests/QuestState.h"
#include "raylib.h"
namespace fw { void DrawQuestLogPanel(const std::vector<QuestEntry>& quests, bool visible){ if(!visible) return; DrawRectangle(1160,20,400,320,Fade(BLACK,0.72f)); DrawRectangleLines(1160,20,400,320,WHITE); DrawText("Quest Log",1180,40,28,LIME); int y=80; for(const auto& q: quests){ const char* state = q.state==QuestProgressState::Completed ? "Completed" : (q.state==QuestProgressState::Active ? "Active":"Not Started"); DrawText(TextFormat("%s [%s]", q.title.empty()?q.id.c_str():q.title.c_str(), state),1180,y,20,WHITE); y += 26; for(const auto& obj: q.objectives){ DrawText(TextFormat("- %s %i/%i", obj.id.c_str(), obj.current, obj.required),1195,y,18,GRAY); y += 22; } y += 8; if(y > 300) break; } } }
