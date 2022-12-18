#include "Logic.hpp"

#include <assert.h>

#include "UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"
#include <algorithm>
#include <iostream>
#include <ranges>

#include "Nodes/ImageViewNode/ImageViewNode.hpp"
#include "Nodes/ReadImageNode/ReadImageNode.hpp"

void Logic::init()
{
    m_idProvider = std::make_unique<SimpleIDProvider>();

    m_nodeCreators["ImageView"] = [&]() { return std::make_unique<ImageViewNode>(m_idProvider.get()); };
    m_nodeCreators["ImageRead"] = [&]() { return std::make_unique<ReadImageNode>(m_idProvider.get()); };

    m_nodes.emplace_back(std::make_unique<ReadImageNode>(m_idProvider.get()));
    m_nodes.emplace_back(std::make_unique<ImageViewNode>(m_idProvider.get()));
}

void Logic::chainReaction(Pin *outputPin)
{
    outputPin->calculate();

    std::ranges::for_each(outputPin->connectedPins, [&](Pin *inputPin) {
        inputPin->payload = outputPin->payload;

        const auto node = inputPin->parentNode;

        if (std::ranges::all_of(node->m_inputPins, [](std::unique_ptr<Pin> &pin) { return pin->payload.has_value(); }))
        {
            node->calculate();

            if (node->m_outputPins.size())
            {
                node->populateOutputPins();
                std::ranges::for_each(node->m_outputPins, [&](std::unique_ptr<Pin> &pin) { chainReaction(pin.get()); });
            }
        }
    });
}

void Logic::propagateEvaluationThroughTheNodes()
{
    auto startChainReaction{[&](const std::unique_ptr<Node> &node) {
        node->calculate();
        node->populateOutputPins();
        std::ranges::for_each(node->m_outputPins, [&](std::unique_ptr<Pin> &pin) { chainReaction(pin.get()); });
    }};

    auto inputNodes = m_nodes | std::ranges::views::filter(
                                    [&](const std::unique_ptr<Node> &node) { return isNodeAnInput(node.get()); });

    cleanNonInputNodesPins();
    std::cout << "Evaluation: Started\n\n";

    std::ranges::for_each(inputNodes, startChainReaction);

    std::cout << "Evaluation: Finished\n\n";
}

bool Logic::isLinkPossible(std::pair<IDType, IDType> pinIdPair)
{
    auto firstPin{getPinById(pinIdPair.first)};
    auto secondPin{getPinById(pinIdPair.second)};

    auto triesToConnectItself{[&]() -> bool { return firstPin->id == secondPin->id; }};
    auto areSameTypes{[&]() -> bool { return typeid(firstPin) == typeid(secondPin); }};

    auto arePinsInputAndOutput{[&]() { return isInputPin(firstPin) != isInputPin(secondPin); }};

    return !triesToConnectItself() && !arePinsAlreadyConnected(firstPin, secondPin) &&
           !arePinsOnSameNode(firstPin, secondPin) && arePinsInputAndOutput() && areSameTypes();
}

void Logic::createLink(std::pair<IDType, IDType> pinIdPair)
{
    auto firstPin{getPinById(pinIdPair.first)};
    auto secondPin{getPinById(pinIdPair.second)};

    m_links.emplace_back(std::make_unique<LinkInfo>(m_idProvider->generateID(), pinIdPair));

    firstPin->connectedPins.emplace_back(secondPin);
    secondPin->connectedPins.emplace_back(firstPin);
}

void Logic::addNode(std::unique_ptr<Node> newNode)
{
    m_nodes.emplace_back(std::move(newNode));
}

void Logic::deleteLink(IDType linkId)
{
    const auto &linkInfo  = getLinkInfoById(linkId);
    auto        firstPin  = getPinById(linkInfo->pinIds.first);
    auto        secondPin = getPinById(linkInfo->pinIds.second);

    auto [removeBeginForFirst, removeEndForFirst] =
        std::ranges::remove_if(firstPin->connectedPins, [&](const Pin *connectedPinToFirst) {
            return connectedPinToFirst->id == secondPin->id;
        });

    firstPin->connectedPins.erase(removeBeginForFirst, removeEndForFirst);

    auto [removeBeginForSecond, removeEndForSecond] =
        std::ranges::remove_if(secondPin->connectedPins, [&](const Pin *connectedPinToSecond) {
            return connectedPinToSecond->id == firstPin->id;
        });

    secondPin->connectedPins.erase(removeBeginForSecond, removeEndForSecond);

    auto [removeBeginForLink, removeEndForLink] = std::ranges::remove(m_links, linkInfo->id, &LinkInfo::id);

    m_links.erase(removeBeginForLink, removeEndForLink);
}

void Logic::deleteNode(IDType nodeId)
{
    Node                   *node{getNodeById(nodeId)};
    std::vector<LinkInfo *> nodeLinks;

    std::ranges::for_each(node->m_inputPins, [&](const std::unique_ptr<Pin> &inputPin) {
        std::ranges::for_each(getPinLinks(inputPin.get()),
                              [&](LinkInfo *linkInfo) { nodeLinks.emplace_back(linkInfo); });
    });

    std::ranges::for_each(node->m_outputPins, [&](const std::unique_ptr<Pin> &outputPin) {
        std::ranges::for_each(getPinLinks(outputPin.get()),
                              [&](LinkInfo *linkInfo) { nodeLinks.emplace_back(linkInfo); });
    });

    std::ranges::for_each(nodeLinks, [&](LinkInfo *linkInfo) { deleteLink(linkInfo->id); });

    auto [beginRemoveNodes, endRemoveNodes] = std::ranges::remove(m_nodes, node->id, &Node::id);

    m_nodes.erase(beginRemoveNodes, endRemoveNodes);
}

const std::vector<std::unique_ptr<LinkInfo>> &Logic::getLinks()
{
    return m_links;
}

const std::vector<std::unique_ptr<Node>> &Logic::getNodes()
{
    return m_nodes;
}

Pin *Logic::getPinById(IDType id) const
{
    auto ptrs = collectAllPins();
    auto it   = std::find_if(std::begin(ptrs), std::end(ptrs), [&](const Pin *pin) { return pin->id == id; });

    assert(it != std::end(ptrs), "Drawing representation data missmatch FATAL!!!");

    return *it;
}

Node *Logic::getNodeById(IDType id) const
{
    for (auto it = std::begin(m_nodes); it < std::end(m_nodes); it++)
    {
        if (it->get()->id == id)
        {
            return it->get();
        }
    }
}

std::vector<LinkInfo *> Logic::getPinLinks(Pin *pin) const
{
    std::vector<LinkInfo *> linksInfo;
    std::ranges::for_each(m_links, [&](const std::unique_ptr<LinkInfo> &linkInfo) {
        if (pin->id == linkInfo->pinIds.first || pin->id == linkInfo->pinIds.second)
        {
            linksInfo.emplace_back(linkInfo.get());
        }
    });

    return linksInfo;
}

bool Logic::isPinOnNode(const Node *node, const Pin *pin) const
{
    return pin->parentNode == node;
}

std::vector<Pin *> Logic::getPinsOnNode(const Node *node) const
{
    std::vector<Pin *> pinsOnNode;

    std::ranges::for_each(node->m_inputPins,
                          [&](const std::unique_ptr<Pin> &pin) { pinsOnNode.emplace_back(pin.get()); });
    std::ranges::for_each(node->m_outputPins,
                          [&](const std::unique_ptr<Pin> &pin) { pinsOnNode.emplace_back(pin.get()); });

    return pinsOnNode;
}

bool Logic::arePinsAlreadyConnected(const Pin *first, const Pin *second) const
{
    return std::ranges::any_of(first->connectedPins,
                               [&](const Pin *connectedPin) { return connectedPin->id == second->id; });
}

bool Logic::arePinsOnSameNode(const Pin *first, const Pin *second) const
{
    return first->parentNode == second->parentNode;
}

bool Logic::isInputPin(const Pin *pin) const
{
    return std::ranges::any_of(pin->parentNode->m_inputPins,
                               [&](const std::unique_ptr<Pin> &nodePin) { return nodePin.get() == pin; });
}

const std::unique_ptr<Node> &Logic::getNodeWithPin(IDType pinId) const
{
    auto it = std::ranges::find_if(m_nodes, [&](const std::unique_ptr<Node> &node) {
        return std::ranges::any_of(getPinsOnNode(node.get()), [&](const Pin *pin) { return pin->id == pinId; });
    });
    assert(it != std::end(m_nodes), "PIN NOT CONNECTED TO NODE FATAL ERROR!!!");
    return *it;
}

void Logic::cleanNonInputNodesPins()
{
    auto nonInputNodes{
        m_nodes | std::ranges::views::filter([&](std::unique_ptr<Node> &node) { return !isNodeAnInput(node.get()); })};

    std::ranges::for_each(nonInputNodes, [](std::unique_ptr<Node> &node) {
        std::ranges::for_each(node->m_inputPins, [](std::unique_ptr<Pin> &pin) { pin->payload.reset(); });
        std::ranges::for_each(node->m_outputPins, [](std::unique_ptr<Pin> &pin) { pin->payload.reset(); });
    });
}

const LinkInfo *Logic::getLinkInfoById(IDType linkId) const
{
    auto it = std::ranges::find_if(m_links,
                                   [&](const std::unique_ptr<LinkInfo> &linkInfo) { return linkInfo->id == linkId; });

    assert(it != std::end(m_links), "Drawing representation data missmatch FATAL!!!");

    return it->get();
}

bool Logic::areNodesConnected(const Node *first, const Node *second) const
{
    auto firstPins{getPinsOnNode(first)};
    auto secondPins{getPinsOnNode(second)};

    return std::ranges::any_of(firstPins, [&](const Pin *firstPin) {
        return std::ranges::any_of(firstPin->connectedPins, [&](const Pin *pinConnectedToFirst) {
            return std::ranges::any_of(secondPins,
                                       [&](const Pin *secondPin) { return pinConnectedToFirst->id == secondPin->id; });
        });
    });
}

std::vector<Pin *> Logic::collectAllPins() const
{
    std::vector<Pin *> allPins;

    for (const auto &node : m_nodes)
    {
        std::ranges::for_each(node->m_inputPins,
                              [&](const std::unique_ptr<Pin> &pin) { allPins.push_back(pin.get()); });

        std::ranges::for_each(node->m_outputPins,
                              [&](const std::unique_ptr<Pin> &pin) { allPins.push_back(pin.get()); });
    }

    return allPins;
}

bool Logic::isNodeAnInput(const Node *node) const
{
    return !node->m_inputPins.size();
}
