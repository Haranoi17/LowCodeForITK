#include "Application/Drawing/DrawStrategies/NodeDrawStrategy.hpp"
#include "Application/Drawing/DrawDefaults.hpp"

#include "imgui_node_editor.h"
namespace ed = ax::NodeEditor;

NodeDrawStrategy::NodeDrawStrategy(Node *node, TextureOperator *textureOperator, TexturesRepository *textureRepository)
    : node{node}, nodeToDrawID{node->id}, textureOperator{textureOperator}, textureRepository{textureRepository}
{
}

void NodeDrawStrategy::draw()
{
    defaultNodeBegin(node);
    auto pos = ed::GetNodePosition(node->id);
    node->setPosition(Position{pos.x, pos.y});
    ed::EndNode();
}
