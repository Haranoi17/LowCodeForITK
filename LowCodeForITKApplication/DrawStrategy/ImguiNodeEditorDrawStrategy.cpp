#include "ImguiNodeEditorDrawStrategy.hpp"
#include "imgui_node_editor.h"
#include <algorithm>

namespace ed = ax::NodeEditor;

void ImguiNodeEditorDrawStrategy::drawNode(const std::unique_ptr<Node>& node) const
{
	ed::BeginNode(node->id);

	ImGui::Text("Node A");

	std::ranges::for_each(node->m_inputPins, [this](const std::shared_ptr<Pin>& pin) {drawInputPin(pin);});

	ImGui::SameLine();

	std::ranges::for_each(node->m_outputPins, [this](const std::shared_ptr<Pin>& pin) {drawOutputPin(pin);});

	ed::EndNode();
}

void ImguiNodeEditorDrawStrategy::drawInputPin(const std::shared_ptr<Pin>& pin) const
{
	ed::BeginPin(pin->id, ed::PinKind::Input);

	ImGui::Text("-> In");
	ed::EndPin();
}

void ImguiNodeEditorDrawStrategy::drawOutputPin(const std::shared_ptr<Pin>& pin) const
{
	ed::BeginPin(pin->id, ed::PinKind::Output);
	ImGui::Text("Out ->");
	ed::EndPin();
}
