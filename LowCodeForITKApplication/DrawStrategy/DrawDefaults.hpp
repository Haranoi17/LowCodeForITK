#pragma once
#include "Nodes\Interface\Node.hpp"
#include "imgui.h"

void drawInputPin(const Pin *pin);

void drawOutputPin(const Pin *pin);

void defaultNodeBegin(const Node *node);

inline float   g_pinIconSize       = 10.0f;
inline ImColor g_pinIconColor      = ImColor(100, 100, 100, 200);
inline ImColor g_pinIconInnerColor = ImColor(50, 50, 50, 200);