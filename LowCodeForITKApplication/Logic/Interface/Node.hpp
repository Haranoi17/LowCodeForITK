#pragma once
#include <vector>
#include <memory>

#include "Pin.hpp"
#include <iostream>

class Node
{
public:
	Node(std::unique_ptr<UniqueIDProvider>& idProvider) :id{ idProvider->generateID() } {}
	std::vector<std::shared_ptr<Pin>> m_inputPins;
	std::vector<std::shared_ptr<Pin>> m_outputPins;

	virtual void calculate() { std::cout << name << " node calculation\n"; };
	virtual void populateOutputPins() { std::cout << name << " populating output pins \n"; };

	IDType id;
	std::string name;
};
