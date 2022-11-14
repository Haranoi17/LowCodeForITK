#include "SampleNode.hpp"
#include <imgui_node_editor.h>
#include <iostream>

namespace ed = ax::NodeEditor;

void SampleNode::callITKStrategy()
{
	std::cout << "Generating code\n";
}

void SampleNode::draw()
{
	ed::BeginNode(m_idProvider->generateID());
	ImGui::Text("Node A");
	ed::BeginPin(m_idProvider->generateID(), ed::PinKind::Input);
	ImGui::Text("-> In");
	ed::EndPin();
	ImGui::SameLine();
	ed::BeginPin(m_idProvider->generateID(), ed::PinKind::Output);
	ImGui::Text("Out ->");
	ed::EndPin();
	ed::EndNode();
}
