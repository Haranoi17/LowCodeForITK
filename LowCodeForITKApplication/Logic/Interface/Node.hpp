#pragma once
#include <vector>
#include <memory>

#include "Pin.hpp"

class Node
{
public:
	Node(std::unique_ptr<UniqueIDProvider>& idProvider) :id{ idProvider->generateID() } {}
	std::vector<std::shared_ptr<Pin>> m_inputPins;
	std::vector<std::shared_ptr<Pin>> m_outputPins;

	IDType id;
};
