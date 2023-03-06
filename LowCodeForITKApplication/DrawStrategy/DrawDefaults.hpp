#pragma once
#include "Nodes\Interface\Node.hpp"
#include "imgui.h"

#ifdef _DEBUG
#define DEBUG_RECTANGLE() \
    ImGui::GetWindowDrawList()->AddRect(ImGui::GetItemRectMin(), ImGui::GetItemRectMax(), IM_COL32(255, 0, 0, 255));
#else
#define DEBUG_RECTANGLE() 0
#endif

void drawInputPin(const Pin *pin);

void drawOutputPin(const Pin *pin);

void defaultNodeBegin(const Node *node);

inline float   g_pinIconSize       = 20.0f;
inline ImColor g_pinIconColor      = ImColor(100, 100, 100, 200);
inline ImColor g_pinIconInnerColor = ImColor(50, 50, 50, 200);