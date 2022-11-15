#include "Node.hpp"

class InputActionNode : public Node
{
public:
	InputActionNode();
};

class BranchNode : public Node
{
public:
	BranchNode();
};

class DoNNode : public Node
{
public:
	DoNNode();
};

class OutputActionNode : public Node
{
public:
	OutputActionNode();
};

class PrintStringNode : public Node
{
public:
	PrintStringNode();
};

class SpawnMessageNode : public Node
{
public:
	SpawnMessageNode();
};

class SetTimerNode : public Node
{
public:
	SetTimerNode();
};

class LessNode : public Node
{
public:
	LessNode();
};

class WeirdNode : public Node
{
	WeirdNode();
};

class TraceByChannelNode : public Node
{
public:
	TraceByChannelNode();
};

class TreeSequenceNode : public Node {
public:
	TreeSequenceNode();
};

class TreeTaskNode : public Node {
public:
	TreeTaskNode();
};

class TreeTask2Node : public Node {
public:
	TreeTask2Node();
};

class CommentNode : public Node {
public:
	CommentNode();
};

class HoudiniTransformNode : public Node {
public:
	HoudiniTransformNode();
};

class HoudiniGroupNode : public Node {
public:
	HoudiniGroupNode();
};
