#include "Logic.hpp"

#include <assert.h>

#include "UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"
#include <algorithm>
#include <iostream>
#include <ranges>

#include "DrawStrategy/BlueprintNodeDrawStarategy.hpp"
#include "DrawStrategy/ImageReadNodeDrawStrategy.hpp"
#include "DrawStrategy/ImageViewNodeDrawStrategy.hpp"
#include "DrawStrategy/RGBANodeDrawStrategy.hpp"

#include "Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Nodes/ImageViewNode/ImageViewNode.hpp"
#include "Nodes/RGBANode/RGBANode.hpp"

std::map<std::string, std::function<std::unique_ptr<Node>()>> Logic::nodeTypeNameToFactoryMethodMap{
    {RGBANode::typeName, []() { return std::make_unique<RGBANode>(); }},
    {ImageReadNode::typeName, []() { return std::make_unique<ImageReadNode>(); }},
    {ImageViewNode::typeName, []() { return std::make_unique<ImageViewNode>(); }}};

std::map<std::string, std::function<std::unique_ptr<NodeDrawStrategy>(Node *)>> Logic::nodeTypeNameToDrawStrategyMap{
    {RGBANode::typeName,
     [](Node *nodePtr) { return std::make_unique<RGBANodeDrawStrategy>(dynamic_cast<RGBANode *>(nodePtr)); }},
    {ImageReadNode::typeName,
     [](Node *nodePtr) { return std::make_unique<ImageReadNodeDrawStrategy>(dynamic_cast<ImageReadNode *>(nodePtr)); }},
    {ImageViewNode::typeName, [](Node *nodePtr) {
         return std::make_unique<ImageViewNodeDrawStrategy>(dynamic_cast<ImageViewNode *>(nodePtr));
     }}};

Logic::Logic() : m_idProvider{std::make_unique<SimpleIDProvider>()}
{
}

void Logic::updateCreators()
{
    m_nodeCreators[ImageViewNode::typeName] = [&]() {
        auto node         = std::make_unique<ImageViewNode>(m_idProvider.get());
        auto drawStrategy = std::make_unique<ImageViewNodeDrawStrategy>(node.get());
        return std::make_unique<NodeWithDrawStrategy>(std::move(node), std::move(drawStrategy));
    };

    m_nodeCreators[ImageReadNode::typeName] = [&]() {
        auto node         = std::make_unique<ImageReadNode>(m_idProvider.get());
        auto drawStrategy = std::make_unique<ImageReadNodeDrawStrategy>(node.get());
        return std::make_unique<NodeWithDrawStrategy>(std::move(node), std::move(drawStrategy));
    };

    m_nodeCreators[RGBANode::typeName] = [&]() {
        auto node         = std::make_unique<RGBANode>(m_idProvider.get());
        auto drawStrategy = std::make_unique<RGBANodeDrawStrategy>(node.get());
        return std::make_unique<NodeWithDrawStrategy>(std::move(node), std::move(drawStrategy));
    };
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

    std::ranges::for_each(inputNodes, startChainReaction);
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

void Logic::addNodeDrawStrategy(std::unique_ptr<NodeDrawStrategy> nodeDrawStrategy)
{
    m_drawNodeStrategies.push_back(std::move(nodeDrawStrategy));
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

    auto [beginRemoveDrawStrategy, endRemoveDrawStrategy] =
        std::ranges::remove(m_drawNodeStrategies, node->id, &NodeDrawStrategy::nodeToDrawID);
    m_drawNodeStrategies.erase(beginRemoveDrawStrategy, endRemoveDrawStrategy);

    auto [beginRemoveNodes, endRemoveNodes] = std::ranges::remove(m_nodes, node->id, &Node::id);
    m_nodes.erase(beginRemoveNodes, endRemoveNodes);
}

const std::vector<std::unique_ptr<LinkInfo>> &Logic::getLinks()
{
    return m_links;
}

json Logic::serialize()
{
    json serializedLogic;

    serializedLogic["idProvider"] = m_idProvider->serialize();
    serializedLogic["links"]      = serializeLinks();
    serializedLogic["nodes"]      = serializeNodes();
    return serializedLogic;
}

std::vector<IDType> recursiveSearch(const json &json)
{
    std::vector<IDType> ids;
    for (const auto &subJson : json)
    {
        if (subJson.contains("id"))
        {
            ids.emplace_back(subJson["id"]);
        }

        if (subJson.is_structured())
        {
            auto found = recursiveSearch(subJson);
            ids.reserve(ids.size() + std::distance(found.begin(), found.end()));
            ids.insert(ids.end(), found.begin(), found.end());
        }
    }

    return ids;
}
void Logic::deserialize(json data)
{
    clearAll();
    deserializeNodes(data["nodes"]);
    deserializeLinks(data["links"]);

    updatePinsAfterDeserialization();

    m_idProvider = std::make_unique<SimpleIDProvider>();
    m_idProvider->deserialize(data["idProvider"]);
}

json Logic::serializeLinks()
{
    json serializedLinks;
    std::ranges::for_each(
        getLinks(), [&](const std::unique_ptr<LinkInfo> &link) { serializedLinks.emplace_back(link->serialize()); });

    return serializedLinks;
}

json Logic::serializeNodes()
{
    json serializedNodes;
    std::ranges::for_each(getNodes(),
                          [&](const std::unique_ptr<Node> &node) { serializedNodes.emplace_back(node->serialize()); });
    return serializedNodes;
}

void Logic::deserializeLinks(json links)
{
    for (auto link : links)
    {
        auto linkObject = std::make_unique<LinkInfo>();
        linkObject->deserialize(link);

        m_links.emplace_back(std::move(linkObject));
    }
}

void Logic::deserializeNodes(json nodes)
{
    for (auto nodeData : nodes)
    {
        std::string nodeTypeName = nodeData["name"];
        auto        node         = nodeTypeNameToFactoryMethodMap.at(nodeTypeName)();
        node->deserialize(nodeData);

        auto drawStrategy = nodeTypeNameToDrawStrategyMap.at(nodeTypeName)(node.get());

        addNode(std::move(node));
        addNodeDrawStrategy(std::move(drawStrategy));
    }
}

void Logic::updatePinsAfterDeserialization()
{
    auto        pins  = collectAllPins();
    const auto &links = getLinks();

    std::ranges::for_each(pins, [&](Pin *currentPin) {
        auto isCurrentPinInLink = [&](const std::unique_ptr<LinkInfo> &link) {
            return currentPin->id == link->pinIds.first || currentPin->id == link->pinIds.second;
        };
        auto linksWithThisPin = links | std::views::filter(isCurrentPinInLink);

        auto updateCurrentPinConncetedPins = [&](const std::unique_ptr<LinkInfo> &link) {
            auto otherPinId = currentPin->id == link->pinIds.first ? link->pinIds.second : link->pinIds.first;
            auto otherPin   = getPinById(otherPinId);

            auto isPinInVector = [](std::vector<Pin *> pins, Pin *pin) {
                return std::ranges::any_of(pins, [&](Pin *pinFromVector) { return pin == pinFromVector; });
            };

            if (!isPinInVector(currentPin->connectedPins, otherPin))
            {
                currentPin->connectedPins.push_back(otherPin);
            }
        };

        std::ranges::for_each(linksWithThisPin, updateCurrentPinConncetedPins);
        currentPin->parentNode = getNodeWithPin(currentPin->id).get();
    });
}

const std::vector<std::unique_ptr<NodeDrawStrategy>> &Logic::getNodesDrawStrategies()
{
    return m_drawNodeStrategies;
}

Pin *Logic::getPinById(IDType id) const
{
    auto ptrs = collectAllPins();
    auto it   = std::find_if(std::begin(ptrs), std::end(ptrs), [&](const Pin *pin) { return pin->id == id; });

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
    return node->m_inputPins.empty();
}
