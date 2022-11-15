#include "NodesCreator.hpp"
#include "Nodes.hpp"

NodeCreator::NodeCreator(std::shared_ptr<UniqueIDProvider> idProvider)
	:m_idProvider{ idProvider }
{
}

std::unique_ptr<Node> NodeCreator::SpawnInputActionNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 128, 128);
	node->Name = "InputAction Fire";
	node->Type = NodeType::Blueprint;

	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Delegate);
	node->Outputs.emplace_back(m_idProvider->generateID(), "Pressed", PinType::Flow);
	node->Outputs.emplace_back(m_idProvider->generateID(), "Released", PinType::Flow);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnBranchNode()
{
	auto node{ std::make_unique<BranchNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 255, 255);
	node->Name = "Branch";
	node->Type = NodeType::Blueprint;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Condition", PinType::Bool);
	node->Outputs.emplace_back(m_idProvider->generateID(), "True", PinType::Flow);
	node->Outputs.emplace_back(m_idProvider->generateID(), "False", PinType::Flow);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnDoNNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 255, 255);
	node->Name = "Do N";
	node->Type = NodeType::Blueprint;

	node->Inputs.emplace_back(m_idProvider->generateID(), "Enter", PinType::Flow);
	node->Inputs.emplace_back(m_idProvider->generateID(), "N", PinType::Int);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Reset", PinType::Flow);
	node->Outputs.emplace_back(m_idProvider->generateID(), "Exit", PinType::Flow);
	node->Outputs.emplace_back(m_idProvider->generateID(), "Counter", PinType::Int);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnOutputActionNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 255, 255);
	node->Name = "OutputAction";
	node->Type = NodeType::Blueprint;

	node->Inputs.emplace_back(m_idProvider->generateID(), "Sample", PinType::Float);
	node->Outputs.emplace_back(m_idProvider->generateID(), "Condition", PinType::Bool);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Event", PinType::Delegate);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnPrintStringNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 255, 255);
	node->Name = "Print String";
	node->Type = NodeType::Blueprint;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);
	node->Inputs.emplace_back(m_idProvider->generateID(), "In String", PinType::String);
	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnMessageNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(128, 195, 248);
	node->Name = "";
	node->Type = NodeType::Simple;

	node->Outputs.emplace_back(m_idProvider->generateID(), "Message", PinType::String);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnSetTimerNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(128, 195, 248);
	node->Name = "Set Timer";
	node->Type = NodeType::Blueprint;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Object", PinType::Object);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Function Name", PinType::Function);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Time", PinType::Float);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Looping", PinType::Bool);
	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnLessNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(128, 195, 248);
	node->Name = "<";
	node->Type = NodeType::Simple;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Float);
	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Float);
	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Float);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnWeirdNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(128, 195, 248);
	node->Name = "o.O";
	node->Type = NodeType::Simple;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Float);
	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Float);
	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Float);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnTraceByChannelNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 128, 64);
	node->Name = "Single Line Trace by Channel";
	node->Type = NodeType::Blueprint;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Start", PinType::Flow);
	node->Inputs.emplace_back(m_idProvider->generateID(), "End", PinType::Int);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Trace Channel", PinType::Float);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Trace Complex", PinType::Bool);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Actors to Ignore", PinType::Int);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Draw Debug Type", PinType::Bool);
	node->Inputs.emplace_back(m_idProvider->generateID(), "Ignore Self", PinType::Bool);
	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);
	node->Outputs.emplace_back(m_idProvider->generateID(), "Out Hit", PinType::Float);
	node->Outputs.emplace_back(m_idProvider->generateID(), "Return Value", PinType::Bool);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnTreeSequenceNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 255, 255);
	node->Name = "Sequence";
	node->Type = NodeType::Tree;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);
	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnTreeTaskNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 255, 255);
	node->Name = "Move To";
	node->Type = NodeType::Tree;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnTreeTask2Node()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 128, 128);
	node->Name = "Random Wait";
	node->Type = NodeType::Tree;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnComment()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 128, 128);
	node->Name = "Test Comment";
	node->Type = NodeType::Comment;
	node->Size = ImVec2(300, 200);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnHoudiniTransformNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 128, 128);
	node->Name = "Transform";
	node->Type = NodeType::Houdini;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);
	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}

std::unique_ptr<Node> NodeCreator::SpawnHoudiniGroupNode()
{
	auto node{ std::make_unique<InputActionNode>() };

	node->ID = m_idProvider->generateID();
	node->Color = ImColor(255, 128, 128);
	node->Name = "Group";
	node->Type = NodeType::Houdini;

	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);
	node->Inputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);
	node->Outputs.emplace_back(m_idProvider->generateID(), "", PinType::Flow);

	for (auto& pin : node->Inputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Input;
	}

	for (auto& pin : node->Outputs)
	{
		pin.Node = node.get();
		pin.Kind = PinKind::Output;
	}

	return node;
}
