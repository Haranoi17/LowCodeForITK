#pragma once
#include "Interface/Node.hpp"

class SampleNode : public Node
{
public:
	SampleNode(std::unique_ptr<UniqueIDProvider>& idProvider) :Node{ idProvider }
	{
		name = "SampleNode";
		m_inputPins.emplace_back(std::make_shared<Pin>(idProvider));
		m_outputPins.emplace_back(std::make_shared<Pin>(idProvider));
	}
};

class OneOutputPinNode : public Node
{
public:
	OneOutputPinNode(std::unique_ptr<UniqueIDProvider>& idProvider) :Node{ idProvider }
	{
		m_outputPins.emplace_back(std::make_shared<Pin>(idProvider));
	}
};


class TwoOutputPinNode : public Node
{
public:
	TwoOutputPinNode(std::unique_ptr<UniqueIDProvider>& idProvider) :Node{ idProvider }
	{
		m_outputPins.emplace_back(std::make_shared<Pin>(idProvider));
		m_outputPins.emplace_back(std::make_shared<Pin>(idProvider));

	}
};

class IntOutputPinNode : public Node
{
public:
	IntOutputPinNode(std::unique_ptr<UniqueIDProvider>& idProvider) :Node{ idProvider }
	{
		name = "IntOutputPinNode";
		m_outputPins.emplace_back(std::make_shared<IntPin>(idProvider));

	}
};

class IntInputPinNode : public Node
{
public:
	IntInputPinNode(std::unique_ptr<UniqueIDProvider>& idProvider) :Node{ idProvider }
	{
		name = "IntInputPinNode";
		m_inputPins.emplace_back(std::make_shared<IntPin>(idProvider));
	}
};
