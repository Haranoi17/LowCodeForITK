#pragma once
#include "Node.hpp"
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include "utilities/builders.h"
#include "utilities/widgets.h"
#include <imgui_node_editor.h>
#include <vector>
#include <string>
#include <memory>

class NodeCreator
{
public:
	NodeCreator(std::shared_ptr<UniqueIDProvider> idProvider);

	std::unique_ptr<Node> SpawnInputActionNode();

	std::unique_ptr<Node> SpawnBranchNode();

	std::unique_ptr<Node> SpawnDoNNode();

	std::unique_ptr<Node> SpawnOutputActionNode();

	std::unique_ptr<Node> SpawnPrintStringNode();

	std::unique_ptr<Node> SpawnMessageNode();

	std::unique_ptr<Node> SpawnSetTimerNode();

	std::unique_ptr<Node> SpawnLessNode();

	std::unique_ptr<Node> SpawnWeirdNode();

	std::unique_ptr<Node> SpawnTraceByChannelNode();

	std::unique_ptr<Node> SpawnTreeSequenceNode();

	std::unique_ptr<Node> SpawnTreeTaskNode();

	std::unique_ptr<Node> SpawnTreeTask2Node();

	std::unique_ptr<Node> SpawnComment();

	std::unique_ptr<Node> SpawnHoudiniTransformNode();

	std::unique_ptr<Node> SpawnHoudiniGroupNode();

private:
	std::shared_ptr<UniqueIDProvider> m_idProvider;
};