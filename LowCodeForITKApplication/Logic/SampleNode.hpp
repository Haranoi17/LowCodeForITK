#pragma once
#include "Interface/Node.hpp"

class SampleNode : public Node
{
public:
	SampleNode(std::unique_ptr<UniqueIDProvider>& idProvider) :Node{ idProvider }
	{
		m_inputPins.emplace_back(std::make_shared<Pin>(idProvider));
		m_outputPins.emplace_back(std::make_shared<Pin>(idProvider));
	}
};