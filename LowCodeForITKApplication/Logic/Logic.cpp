#include "Logic.hpp"
#include <algorithm>
#include <ranges>

#include "SampleNode.hpp"
#include "Identification/UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"
#include <assert.h>
#include <iostream>

void Logic::init()
{
	m_idProvider = std::make_unique<SimpleIDProvider>();
	m_nodes.emplace_back(std::make_unique<IntOutputPinNode>(m_idProvider));
	m_nodes.emplace_back(std::make_unique<IntInputPinNode>(m_idProvider));
	m_nodes.emplace_back(std::make_unique<SampleNode>(m_idProvider));
}

void Logic::pushResultsToConnectedInputPins(const std::shared_ptr<Pin>& outputPin)
{
	outputPin->calculate();
	std::ranges::for_each(outputPin->connectedPins | std::ranges::views::filter([&](const std::shared_ptr<Pin>& connectedPin) {return isInputPin(connectedPin->id);}), [&](const std::shared_ptr<Pin> inputPin)
		{
			inputPin->value = outputPin->value;
			const auto& node = getNodeWithPin(inputPin->id);
			if (std::ranges::all_of(node->m_inputPins, [](const std::shared_ptr<Pin>& pin)
				{
					return pin->value.has_value();}))
			{
				node->calculate();
				if (node->m_outputPins.size())
				{
					node->populateOutputPins();
					std::ranges::for_each(node->m_outputPins, [&](const std::shared_ptr<Pin>& pin) {pushResultsToConnectedInputPins(pin);});
				}
			}
		});
}

void Logic::propagateEvaluationThroughTheNodes()
{
	auto moveCalculatedOutputsToNextInputs{ [&](const std::unique_ptr<Node>& node)
		{
			node->calculate();
			std::ranges::for_each(node->m_outputPins, [&](const std::shared_ptr<Pin>& pin) {pushResultsToConnectedInputPins(pin);});
		}
	};

	auto inputNodes = m_nodes | std::ranges::views::filter([&](const std::unique_ptr<Node>& node) {return isNodeAnInput(node);});
	std::ranges::for_each(inputNodes, moveCalculatedOutputsToNextInputs);

	std::cout << "END\n\n";
}

bool Logic::isLinkPossible(std::pair<IDType, IDType> pinIdPair)
{
	auto firstPin{ getPinById(pinIdPair.first) };
	auto secondPin{ getPinById(pinIdPair.second) };

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

void Logic::deleteLink(IDType linkId)
{
	const auto& linkInfo = getLinkInfoById(linkId);
	auto firstPin = getPinById(linkInfo->pinIds.first);
	auto secondPin = getPinById(linkInfo->pinIds.second);

	auto [removeBeginForFirst, removeEndForFirst] = std::ranges::remove_if(firstPin->connectedPins, [&](const std::shared_ptr<Pin>& connectedPinToFirst) {return connectedPinToFirst->id == secondPin->id;});
	firstPin->connectedPins.erase(removeBeginForFirst, removeEndForFirst);

	auto [removeBeginForSecond, removeEndForSecond] = std::ranges::remove_if(secondPin->connectedPins, [&](const std::shared_ptr<Pin>& connectedPinToSecond) {return connectedPinToSecond->id == firstPin->id;});
	secondPin->connectedPins.erase(removeBeginForSecond, removeEndForSecond);

	auto [removeBeginForLink, removeEndForLink] = std::ranges::remove_if(m_links, [&](const std::unique_ptr<LinkInfo>& linkInfo) {return linkInfo->id == linkId;});
	m_links.erase(removeBeginForLink, removeEndForLink);
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

	assert(it != std::end(ptrs), "Drawing representation data missmatch FATAL!!!");

	return *it;
}

bool Logic::isPinOnNode(const std::unique_ptr<Node>& node, IDType pinId)
{
	return std::ranges::any_of(getPinsOnNode(node), [&](const std::shared_ptr<Pin>& pin) {return pin->id == pinId;});
}


std::vector<std::shared_ptr<Pin>> Logic::getPinsOnNode(const std::unique_ptr<Node>& node) const
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

bool Logic::isInputPin(IDType pinId) const
{
	return std::ranges::any_of(m_nodes, [&](const std::unique_ptr<Node>& node) {return std::ranges::any_of(node->m_inputPins, [&](const std::shared_ptr<Pin> inputPin) {return inputPin->id == pinId;});});
}

bool Logic::isOutputPin(IDType pinId) const
{
	return std::ranges::any_of(m_nodes, [&](const std::unique_ptr<Node>& node) {return std::ranges::any_of(node->m_outputPins, [&](const std::shared_ptr<Pin> inputPin) {return inputPin->id == pinId;});});
}

const std::unique_ptr<Node>& Logic::getNodeWithPin(IDType pinId)
{
	auto it = std::ranges::find_if(m_nodes, [&](const std::unique_ptr<Node>& node) {return std::ranges::any_of(getPinsOnNode(node), [&](const std::shared_ptr<Pin>& pin) {return pin->id == pinId;});});
	assert(it != std::end(m_nodes), "PIN NOT CONNECTED TO NODE FATAL ERROR!!!");
	return *it;
}

const std::unique_ptr<LinkInfo>& Logic::getLinkInfoById(IDType linkId)
{
	auto it = std::ranges::find_if(m_links, [&](const std::unique_ptr<LinkInfo>& linkInfo) {return linkInfo->id = linkId;});

	assert(it != std::end(m_links), "Drawing representation data missmatch FATAL!!!");

	return *it;
}

bool Logic::areNodesConnected(const std::unique_ptr<Node>& first, const std::unique_ptr<Node>& second) const
{
	auto firstPins{ getPinsOnNode(first) };
	auto secondPins{ getPinsOnNode(second) };

	return std::ranges::any_of(firstPins, [&](const std::shared_ptr<Pin> firstPin) {return std::ranges::any_of(firstPin->connectedPins, [&](const std::shared_ptr<Pin>& pinConnectedToFirst) {return std::ranges::any_of(secondPins, [&](const std::shared_ptr<Pin>& secondPin) {return pinConnectedToFirst->id == secondPin->id;});});});
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

bool Logic::isNodeAnInput(const std::unique_ptr<Node>& node) const
{
	return !node->m_inputPins.size();
}
