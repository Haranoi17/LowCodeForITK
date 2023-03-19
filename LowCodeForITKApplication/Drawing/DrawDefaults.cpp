#include "DrawDefaults.hpp"
#include "imgui_node_editor.h"
#include <format>

namespace ed = ax::NodeEditor;

void drawInputPin(const Pin *pin)
{
    ed::BeginPin(pin->id, ed::PinKind::Input);
    ImGui::Text(std::format("->{}", pin->name).c_str());
    ed::EndPin();
}

void drawOutputPin(const Pin *pin)
{
    ed::BeginPin(pin->id, ed::PinKind::Output);
    ImGui::Text(std::format("{}->", pin->name).c_str());
    ed::EndPin();
}

void defaultNodeBegin(const Node *node)
{
    ed::BeginNode(node->id);
    ImGui::Text(std::format("{}", node->name).c_str());

    ImGui::BeginGroup();
    std::ranges::for_each(node->inputPins, [&](const std::unique_ptr<Pin> &pin) { drawInputPin(pin.get()); });
    ImGui::EndGroup();
    node->inputPins.empty() ? ImGui::SameLine(200) : ImGui::SameLine();
    ImGui::BeginGroup();
    std::ranges::for_each(node->outputPins, [&](const std::unique_ptr<Pin> &pin) { drawOutputPin(pin.get()); });
    ImGui::EndGroup();
}