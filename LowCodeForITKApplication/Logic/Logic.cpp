#include "Logic.hpp"

#include <assert.h>

#include "Logic/utilities.hpp"
#include <algorithm>
#include <iostream>

#include <range/v3/action.hpp>
#include <range/v3/algorithm.hpp>
#include <range/v3/range.hpp>
#include <range/v3/view.hpp>

#include "UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"

#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Drawing/DrawStrategies/EdgeDetectionNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/ImageReadNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/ImageViewNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/RGBANodeDrawStrategy.hpp"

#include "Logic/utilities.hpp"

#include "Nodes/EdgeDetectionNode/EdgeDetectionNode.hpp"
#include "Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Nodes/ImageViewNode/ImageViewNode.hpp"
#include "Nodes/RGBANode/RGBANode.hpp"

std::map<std::string, std::function<std::unique_ptr<Node>()>> Logic::nodeTypeNameToFactoryMethodMap{
    {RGBANode::typeName, []() { return std::make_unique<RGBANode>(); }},
    {ImageReadNode::typeName, []() { return std::make_unique<ImageReadNode>(); }},
    {ImageViewNode::typeName, []() { return std::make_unique<ImageViewNode>(); }},
    {PercentageNode::typeName, []() { return std::make_unique<PercentageNode>(); }},
    {TintNode::typeName, []() { return std::make_unique<TintNode>(); }},
    {EdgeDetectionNode::typeName, []() { return std::make_unique<EdgeDetectionNode>(); }}};

std::map<std::string, std::function<std::unique_ptr<NodeDrawStrategy>(Node *)>> Logic::nodeTypeNameToDrawStrategyMap{
    {RGBANode::typeName,
     [](Node *nodePtr) { return std::make_unique<RGBANodeDrawStrategy>(dynamic_cast<RGBANode *>(nodePtr)); }},
    {ImageReadNode::typeName,
     [](Node *nodePtr) { return std::make_unique<ImageReadNodeDrawStrategy>(dynamic_cast<ImageReadNode *>(nodePtr)); }},
    {ImageViewNode::typeName,
     [](Node *nodePtr) { return std::make_unique<ImageViewNodeDrawStrategy>(dynamic_cast<ImageViewNode *>(nodePtr)); }},
    {PercentageNode::typeName,
     [](Node *nodePtr) {
         return std::make_unique<PercentageNodeDrawStrategy>(dynamic_cast<PercentageNode *>(nodePtr));
     }},
    {TintNode::typeName,
     [](Node *nodePtr) { return std::make_unique<TintNodeDrawStrategy>(dynamic_cast<TintNode *>(nodePtr)); }},
    {EdgeDetectionNode::typeName, [](Node *nodePtr) {
         return std::make_unique<EdgeDetectionNodeDrawStrategy>(dynamic_cast<EdgeDetectionNode *>(nodePtr));
     }}};

Logic::Logic() : idProvider{std::make_unique<SimpleIDProvider>()}
{
}

void Logic::updateCreators()
{
    m_nodeCreators[ImageViewNode::typeName] = [&]() {
        auto node         = std::make_unique<ImageViewNode>(idProvider.get());
        auto drawStrategy = std::make_unique<ImageViewNodeDrawStrategy>(node.get());
        return std::make_unique<NodeWithDrawStrategy>(std::move(node), std::move(drawStrategy));
    };

    m_nodeCreators[ImageReadNode::typeName] = [&]() {
        auto node         = std::make_unique<ImageReadNode>(idProvider.get());
        auto drawStrategy = std::make_unique<ImageReadNodeDrawStrategy>(node.get());
        return std::make_unique<NodeWithDrawStrategy>(std::move(node), std::move(drawStrategy));
    };

    m_nodeCreators[RGBANode::typeName] = [&]() {
        auto node         = std::make_unique<RGBANode>(idProvider.get());
        auto drawStrategy = std::make_unique<RGBANodeDrawStrategy>(node.get());
        return std::make_unique<NodeWithDrawStrategy>(std::move(node), std::move(drawStrategy));
    };

    m_nodeCreators[PercentageNode::typeName] = [&]() {
        auto node         = std::make_unique<PercentageNode>(idProvider.get());
        auto drawStrategy = std::make_unique<PercentageNodeDrawStrategy>(node.get());
        return std::make_unique<NodeWithDrawStrategy>(std::move(node), std::move(drawStrategy));
    };

    m_nodeCreators[TintNode::typeName] = [&]() {
        auto node         = std::make_unique<TintNode>(idProvider.get());
        auto drawStrategy = std::make_unique<TintNodeDrawStrategy>(node.get());
        return std::make_unique<NodeWithDrawStrategy>(std::move(node), std::move(drawStrategy));
    };

    m_nodeCreators[EdgeDetectionNode::typeName] = [&]() {
        auto node         = std::make_unique<EdgeDetectionNode>(idProvider.get());
        auto drawStrategy = std::make_unique<EdgeDetectionNodeDrawStrategy>(node.get());
        return std::make_unique<NodeWithDrawStrategy>(std::move(node), std::move(drawStrategy));
    };
}

void Logic::chainReaction(Pin *outputPin)
{
    outputPin->calculate();
    ranges::for_each(outputPin->connectedPins, [&](Pin *inputPin) {
        inputPin->payload = outputPin->payload;

        const auto node = inputPin->parentNode;

        if (std::ranges::all_of(node->inputPins, [](std::unique_ptr<Pin> &pin) { return pin->payload.has_value(); }))
        {
            node->calculate();

            if (node->outputPins.size())
            {
                node->populateOutputPins();
                std::ranges::for_each(node->outputPins, [&](std::unique_ptr<Pin> &pin) { chainReaction(pin.get()); });
            }
        }
    });
}

void Logic::propagateEvaluationThroughTheNodes()
{
    auto startChainReaction{[&](const std::unique_ptr<Node> &node) {
        node->calculate();
        node->populateOutputPins();
        std::ranges::for_each(node->outputPins, [&](std::unique_ptr<Pin> &pin) { chainReaction(pin.get()); });
    }};

    auto inputNodes = nodes | ranges::views::filter([&](const auto &node) { return isNodeAnInput(node.get()); });

    // cleanNonInputNodesPins();

    std::ranges::for_each(inputNodes, startChainReaction);
}

bool Logic::isLinkPossible(std::pair<IDType, IDType> pinIdPair)
{
    auto firstPin{getPinById(pinIdPair.first)};
    auto secondPin{getPinById(pinIdPair.second)};

    bool triesToConnectItself  = firstPin->id == secondPin->id;
    bool areSameTypes          = firstPin->typeName == secondPin->typeName;
    bool arePinsInputAndOutput = isInputPin(firstPin) != isInputPin(secondPin);

    return !triesToConnectItself && !arePinsAlreadyConnected(firstPin, secondPin) &&
           !arePinsOnSameNode(firstPin, secondPin) && arePinsInputAndOutput && areSameTypes;
}

void Logic::createLink(std::pair<IDType, IDType> pinIdPair)
{
    auto firstPin{getPinById(pinIdPair.first)};
    auto secondPin{getPinById(pinIdPair.second)};

    links.emplace_back(std::make_unique<LinkInfo>(idProvider->generateID(), pinIdPair));

    firstPin->connectedPins.emplace_back(secondPin);
    secondPin->connectedPins.emplace_back(firstPin);
}

void Logic::addNode(std::unique_ptr<Node> newNode)
{
    nodes.emplace_back(std::move(newNode));
}

void Logic::addNodeDrawStrategy(std::unique_ptr<NodeDrawStrategy> nodeDrawStrategy)
{
    nodeDrawStrategies.push_back(std::move(nodeDrawStrategy));
}

bool Logic::innerNodesStateChanged() const
{
    return ranges::any_of(nodes, [](const auto &node) { return node->dirty; });
}

void Logic::removeDirtyFlagsFromNodes()
{
    ranges::for_each(nodes, [](auto &node) { node->dirty = false; });
}

void Logic::deleteLink(IDType linkId)
{
    auto linkInfo  = getLinkInfoById(linkId);
    auto firstPin  = getPinById(linkInfo->pinIds.first);
    auto secondPin = getPinById(linkInfo->pinIds.second);

    auto [removeBeginForFirst, removeEndForFirst] =
        std::ranges::remove_if(firstPin->connectedPins, [&secondPin](const Pin *connectedPinToFirst) {
            return connectedPinToFirst->id == secondPin->id;
        });

    firstPin->connectedPins.erase(removeBeginForFirst, removeEndForFirst);

    auto [removeBeginForSecond, removeEndForSecond] =
        std::ranges::remove_if(secondPin->connectedPins, [&firstPin](const Pin *connectedPinToSecond) {
            return connectedPinToSecond->id == firstPin->id;
        });

    secondPin->connectedPins.erase(removeBeginForSecond, removeEndForSecond);

    auto [removeBeginForLink, removeEndForLink] = std::ranges::remove(links, linkId, &LinkInfo::id);

    links.erase(removeBeginForLink, removeEndForLink);
}

void Logic::deleteNode(IDType nodeId)
{
    auto nodePins = getPinsOnNode(getNodeById(nodeId));

    auto                    toPinLinks = [this](const Pin *pin) { return getPinLinks(pin); };
    std::vector<LinkInfo *> nodeLinks =
        nodePins | ranges::views::transform(toPinLinks) | ranges::actions::join | ranges::to_vector;

    ranges::for_each(nodeLinks, [&](LinkInfo *linkInfo) { deleteLink(linkInfo->id); });

    auto [beginRemoveDrawStrategy, endRemoveDrawStrategy] =
        std::ranges::remove(nodeDrawStrategies, nodeId, &NodeDrawStrategy::nodeToDrawID);
    nodeDrawStrategies.erase(beginRemoveDrawStrategy, endRemoveDrawStrategy);

    auto [beginRemoveNodes, endRemoveNodes] = std::ranges::remove(nodes, nodeId, &Node::id);
    nodes.erase(beginRemoveNodes, endRemoveNodes);
}

std::vector<NodeDrawStrategy *> Logic::getNodesDrawStrategies() const
{
    return nodeDrawStrategies | std::views::transform(ToNonOwningPointer()) | ranges::to_vector;
}

std::vector<LinkInfo *> Logic::getLinks() const
{
    return links | std::views::transform(ToNonOwningPointer()) | ranges::to_vector;
}

std::vector<Node *> Logic::getNodes() const
{
    return nodes | std::views::transform(ToNonOwningPointer()) | ranges::to_vector;
}

json Logic::serialize()
{
    json serializedLogic;

    serializedLogic["idProvider"] = idProvider->serialize();
    serializedLogic["links"]      = serializeLinks();
    serializedLogic["nodes"]      = serializeNodes();
    return serializedLogic;
}

void Logic::deserialize(json data)
{
    clearAll();
    deserializeNodes(data["nodes"]);
    deserializeLinks(data["links"]);

    updatePinsAfterDeserialization();

    idProvider = std::make_unique<SimpleIDProvider>();
    idProvider->deserialize(data["idProvider"]);
}

json Logic::serializeLinks()
{
    json serializedLinks;
    std::ranges::for_each(getLinks(), [&](const auto &link) { serializedLinks.emplace_back(link->serialize()); });

    return serializedLinks;
}

json Logic::serializeNodes()
{
    json serializedNodes;
    std::ranges::for_each(getNodes(), [&](const auto &node) { serializedNodes.emplace_back(node->serialize()); });
    return serializedNodes;
}

void Logic::deserializeLinks(json serializedLinks)
{
    for (auto link : serializedLinks)
    {
        auto linkObject = std::make_unique<LinkInfo>();
        linkObject->deserialize(link);

        links.emplace_back(std::move(linkObject));
    }
}

void Logic::deserializeNodes(json serializedNodes)
{
    for (auto nodeData : serializedNodes)
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
        auto isCurrentPinInLink = [&](const auto &link) {
            return currentPin->id == link->pinIds.first || currentPin->id == link->pinIds.second;
        };
        auto linksWithThisPin = links | std::views::filter(isCurrentPinInLink);

        auto updateCurrentPinConncetedPins = [&](const auto &link) {
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

Pin *Logic::getPinById(IDType id) const
{
    auto ptrs = collectAllPins();
    return *ranges::find_if(ptrs, [&](const Pin *pin) { return pin->id == id; });
}

Node *Logic::getNodeById(IDType nodeId) const
{
    auto hasGivenId = [nodeId](const auto &node) { return node->id == nodeId; };

    return ranges::find_if(nodes, hasGivenId)->get();
}

std::vector<LinkInfo *> Logic::getPinLinks(const Pin *pin) const
{
    return links | ranges::views::transform(ToNonOwningPointer()) |
           ranges::views::filter([pin](const LinkInfo *linkInfo) {
               return pin->id == linkInfo->pinIds.first || pin->id == linkInfo->pinIds.second;
           }) |
           ranges::to_vector;
}

bool Logic::arePinsAlreadyConnected(const Pin *first, const Pin *second) const
{
    return ranges::any_of(first->connectedPins,
                          [&](const Pin *connectedPin) { return connectedPin->id == second->id; });
}

bool Logic::arePinsOnSameNode(const Pin *first, const Pin *second) const
{
    return first->parentNode == second->parentNode;
}

bool Logic::isInputPin(const Pin *pin) const
{
    return ranges::any_of(pin->parentNode->inputPins,
                          [&](const std::unique_ptr<Pin> &nodePin) { return nodePin.get() == pin; });
}

const std::unique_ptr<Node> &Logic::getNodeWithPin(IDType pinId) const
{
    return *ranges::find_if(nodes, [&](const std::unique_ptr<Node> &node) {
        return ranges::any_of(getPinsOnNode(node.get()), [&](const Pin *pin) { return pin->id == pinId; });
    });
}

void Logic::cleanNonInputNodesPins()
{
    auto nonInputNodes{nodes |
                       ranges::views::filter([&](std::unique_ptr<Node> &node) { return !isNodeAnInput(node.get()); })};

    std::ranges::for_each(nonInputNodes, [](std::unique_ptr<Node> &node) {
        std::ranges::for_each(node->inputPins, [](std::unique_ptr<Pin> &pin) { pin->payload.reset(); });
        std::ranges::for_each(node->outputPins, [](std::unique_ptr<Pin> &pin) { pin->payload.reset(); });
    });
}

void Logic::clearAll()
{
    links.clear();
    nodes.clear();
    nodeDrawStrategies.clear();
    idProvider.reset();
}

const LinkInfo *Logic::getLinkInfoById(IDType linkId) const
{
    auto hasGivenId    = [linkId](const auto &linkInfo) { return linkInfo->id == linkId; };
    auto linksPointers = links | ranges::views::transform(ToNonOwningPointer()) | ranges::to_vector;
    return *ranges::find_if(linksPointers, hasGivenId);
}

std::vector<Pin *> Logic::getPinsOnNode(const Node *node) const
{
    return ranges::views::concat(node->inputPins | ranges::views::transform(ToNonOwningPointer()),
                                 node->outputPins | ranges::views::transform(ToNonOwningPointer())) |
           ranges::to_vector;
}

std::vector<Pin *> Logic::collectAllPins() const
{
    auto toPinsOnNode = [this](const auto &node) { return getPinsOnNode(node.get()); };
    return nodes | ranges::views::transform(toPinsOnNode) | ranges::actions::join;
}

bool Logic::isNodeAnInput(const Node *node) const
{
    return node->inputPins.empty();
}
