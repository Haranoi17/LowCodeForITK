#include "Logic.hpp"
#include <algorithm>
#include <ranges>

#include "SampleNode.hpp"
#include "Identification/UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"
#include <assert.h>

void Logic::init()
{
	m_idProvider = std::make_unique<SimpleIDProvider>();
	m_nodes.emplace_back(std::make_unique<SampleNode>(m_idProvider));
	m_nodes.emplace_back(std::make_unique<SampleNode>(m_idProvider));
}

bool Logic::isLinkPossible(std::pair<IDType, IDType> pinIdPair)
{
	auto firstPin{ getPinById(pinIdPair.first) };
	auto secondPin{ getPinById(pinIdPair.second) };

	assert(firstPin && secondPin, "LOGICAL ERROR TIME TO WRITE TESTS!!!");

	auto triesToConnectItself{ [&]()->bool {return firstPin->id == secondPin->id;} };
	auto areSameTypes{ [&]()->bool {return  typeid(firstPin.get()) == typeid(secondPin.get());} };

	auto arePinsInputAndOutput{ [&]() {
		bool isFirstPinInput{false};
		bool isSecondPinInput{ false };
		for (const auto& node : m_nodes)
		{
			auto it = std::ranges::find_if(node->m_inputPins, [&](const std::shared_ptr<Pin>& pin) {return pin->id == firstPin->id;});
			if (it != std::end(node->m_inputPins))
			{
				isFirstPinInput = true;
				break;
			}
		}

		for (const auto& node : m_nodes)
		{
			auto it = std::ranges::find_if(node->m_inputPins, [&](const std::shared_ptr<Pin>& pin) {return pin->id == secondPin->id;});
			if (it != std::end(node->m_inputPins))
			{
				isSecondPinInput = true;
				break;
			}
		}

		return isFirstPinInput != isSecondPinInput;

	} };

	return !triesToConnectItself() && !arePinsAlreadyConnected(firstPin, secondPin) && !arePinsOnSameNode(pinIdPair) && arePinsInputAndOutput() && areSameTypes();
}

void Logic::createLink(std::pair<IDType, IDType> pinIdPair)
{
	auto firstPin{ getPinById(pinIdPair.first) };
	auto secondPin{ getPinById(pinIdPair.second) };

	m_links.emplace_back(std::make_unique<LinkInfo>(m_idProvider->generateID(), pinIdPair));

	firstPin->connectedPins.emplace_back(secondPin);
	secondPin->connectedPins.emplace_back(firstPin);
}

const std::vector<std::unique_ptr<LinkInfo>>& Logic::getLinks()
{
	return m_links;
}

const std::vector<std::unique_ptr<Node>>& Logic::getNodes()
{
	return m_nodes;
}

std::shared_ptr<Pin> Logic::getPinById(IDType id)
{
	auto ptrs = collectAllPins();
	auto it = std::find_if(std::begin(ptrs), std::end(ptrs), [&](const std::shared_ptr<Pin>& pin) {return pin->id == id;});
	if (it == std::end(ptrs))
	{
		return nullptr;
	}

	return *it;
}

bool Logic::isPinOnNode(const std::unique_ptr<Node>& node, IDType pinId)
{
	return std::ranges::any_of(getPinsOnNode(node), [&](const std::shared_ptr<Pin>& pin) {return pin->id == pinId;});
}


std::vector<std::shared_ptr<Pin>> Logic::getPinsOnNode(const std::unique_ptr<Node>& node)
{
	std::vector<std::shared_ptr<Pin>> pinsOnNode;

	std::ranges::for_each(node->m_inputPins, [&](std::shared_ptr<Pin>& pin) {pinsOnNode.push_back(pin);});
	std::ranges::for_each(node->m_outputPins, [&](std::shared_ptr<Pin>& pin) {pinsOnNode.push_back(pin);});

	return pinsOnNode;
}

bool Logic::arePinsAlreadyConnected(const std::shared_ptr<Pin>& first, const std::shared_ptr<Pin>& second)
{
	return std::ranges::any_of(first->connectedPins, [&](const std::shared_ptr<Pin>& connectedPin) {return connectedPin->id == second->id;});
}

bool Logic::arePinsOnSameNode(std::pair<IDType, IDType> pinIdPair)
{
	return std::ranges::any_of(m_nodes, [&](const std::unique_ptr<Node>& node) {return isPinOnNode(node, pinIdPair.first) && isPinOnNode(node, pinIdPair.second);});
}

std::vector<std::shared_ptr<Pin>> Logic::collectAllPins()
{
	std::vector<std::shared_ptr<Pin>> allPins;

	for (const auto& node : m_nodes)
	{
		std::ranges::for_each(node->m_inputPins, [&](std::shared_ptr<Pin>& pin) {allPins.push_back(pin);});
		std::ranges::for_each(node->m_outputPins, [&](std::shared_ptr<Pin>& pin) {allPins.push_back(pin);});
	}

	return allPins;
}
