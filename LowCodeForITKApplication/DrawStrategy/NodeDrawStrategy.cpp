#include "NodeDrawStrategy.hpp"
#include "DrawDefaults.hpp"

#include "imgui_node_editor.h"
namespace ed = ax::NodeEditor;

NodeDrawStrategy::NodeDrawStrategy(Node *node) : m_node{node}, nodeToDrawID{node->id}
{
}

void NodeDrawStrategy::draw()
{
    defaultNodeBegin(m_node);
    ed::EndNode();
}
