#include "Drawing/DrawStrategies/NodeDrawStrategy.hpp"
#include "Drawing/DrawDefaults.hpp"

#include "imgui_node_editor.h"
namespace ed = ax::NodeEditor;

NodeDrawStrategy::NodeDrawStrategy(Node *node) : node{node}, nodeToDrawID{node->id}
{
}

void NodeDrawStrategy::draw()
{
    defaultNodeBegin(node);
    ed::EndNode();
}
