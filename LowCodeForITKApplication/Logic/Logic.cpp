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

#include "Logic/utilities.hpp"

Logic::Logic(NodesDefinitions *nodesDefinitions) : idProvider{std::make_unique<SimpleIDProvider>()}, nodesDefinitions{nodesDefinitions}
{
}

void Logic::chainReaction(Pin *outputPin, bool clear)
{

    outputPin->calculate();
    std::ranges::for_each(outputPin->connectedPins, [&](Pin *inputPin) {
        inputPin->payload = outputPin->payload;

        const auto node = inputPin->parentNode;

        if (std::ranges::all_of(node->inputPins, [](std::unique_ptr<Pin> &pin) { return pin->payload.has_value(); }))
        {
            if (!ranges::contains(alreadyRecalculatedNodes, node->id))
            {
                node->calculate();
                alreadyRecalculatedNodes.push_back(node->id);
            }

            if (node->outputPins.size())
            {
                node->populateOutputPins();
                std::ranges::for_each(node->outputPins, [&](std::unique_ptr<Pin> &pin) { chainReaction(pin.get(), false); });
            }
        }
    });

    if (clear)
    {
        alreadyRecalculatedNodes.clear();
    }
}

void Logic::propagateEvaluationThroughTheNodes()
{
    bool clear = true;
    auto startChainReaction{[&](const std::unique_ptr<Node> &node) {
        node->calculate();
        node->populateOutputPins();
        std::ranges::for_each(node->outputPins, [&](std::unique_ptr<Pin> &pin) {
            chainReaction(pin.get(), clear);
            clear = false;
        });
    }};

    auto inputNodes = nodes | ranges::views::filter([&](const auto &node) { return isNodeAnInput(node.get()); });

    std::ranges::for_each(inputNodes, startChainReaction);
}

bool Logic::isLinkPossible(std::pair<IDType, IDType> pinIdPair)
{
    auto firstPin{getPinById(pinIdPair.first)};
    auto secondPin{getPinById(pinIdPair.second)};

    bool triesToConnectItself  = firstPin->id == secondPin->id;
    bool areSameTypes          = firstPin->typeName == secondPin->typeName;
    bool arePinsInputAndOutput = isInputPin(firstPin) != isInputPin(secondPin);

    return !triesToConnectItself && !arePinsAlreadyConnected(firstPin, secondPin) && !arePinsOnSameNode(firstPin, secondPin) &&
           arePinsInputAndOutput && areSameTypes;
}

void Logic::createLink(std::pair<IDType, IDType> pinIdPair)
{
    auto firstPin{getPinById(pinIdPair.first)};
    auto secondPin{getPinById(pinIdPair.second)};

    links.emplace_back(std::make_unique<Link>(idProvider.get(), pinIdPair));

    firstPin->connectedPins.emplace_back(secondPin);
    secondPin->connectedPins.emplace_back(firstPin);
}

bool Logic::isInputPin(IDType pinId)
{
    return isInputPin(getPinById(pinId));
}

void Logic::addNode(std::unique_ptr<Node> newNode)
{
    nodes.emplace_back(std::move(newNode));
}

std::string Logic::getPinType(IDType pinId)
{
    return getPinById(pinId)->typeName;
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
    auto link      = getLinkById(linkId);
    auto firstPin  = getPinById(link->pinIds.first);
    auto secondPin = getPinById(link->pinIds.second);

    auto [removeBeginForFirst, removeEndForFirst] = std::ranges::remove_if(
        firstPin->connectedPins, [&secondPin](const Pin *connectedPinToFirst) { return connectedPinToFirst->id == secondPin->id; });

    firstPin->connectedPins.erase(removeBeginForFirst, removeEndForFirst);

    auto [removeBeginForSecond, removeEndForSecond] = std::ranges::remove_if(
        secondPin->connectedPins, [&firstPin](const Pin *connectedPinToSecond) { return connectedPinToSecond->id == firstPin->id; });

    secondPin->connectedPins.erase(removeBeginForSecond, removeEndForSecond);

    auto [removeBeginForLink, removeEndForLink] = std::ranges::remove(links, linkId, &Link::id);

    links.erase(removeBeginForLink, removeEndForLink);
}

void Logic::deleteNode(IDType nodeId)
{
    auto nodePins = getPinsOnNode(getNodeById(nodeId));

    auto                toPinLinks = [this](const Pin *pin) { return getPinLinks(pin); };
    std::vector<Link *> nodeLinks  = nodePins | ranges::views::transform(toPinLinks) | ranges::actions::join | ranges::to_vector;

    ranges::for_each(nodeLinks, [&](Link *link) { deleteLink(link->id); });

    auto [beginRemoveNodes, endRemoveNodes] = std::ranges::remove(nodes, nodeId, &Node::id);
    nodes.erase(beginRemoveNodes, endRemoveNodes);
}

std::vector<Link *> Logic::getLinks() const
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

    idProvider = std::make_unique<SimpleIDProvider>();
    idProvider->deserialize(data["idProvider"]);

    deserializeNodes(data["nodes"]);
    deserializeLinks(data["links"]);

    updatePinsAfterDeserialization();
}

Node *Logic::getLastAddedNode()
{
    return nodes.back().get();
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
        auto linkObject = std::make_unique<Link>(getIdProvider());
        linkObject->deserialize(link);

        links.emplace_back(std::move(linkObject));
    }
}

void Logic::deserializeNodes(json serializedNodes)
{
    for (auto nodeData : serializedNodes)
    {
        std::string nodeTypeName = nodeData["name"];
        auto        node         = nodesDefinitions->getMapOfNodeCreators().at(nodeTypeName)->createFullyFunctional(getIdProvider());
        node->deserialize(nodeData);

        addNode(std::move(node));
    }
}

void Logic::updatePinsAfterDeserialization()
{
    auto        pins  = collectAllPins();
    const auto &links = getLinks();

    std::ranges::for_each(pins, [&](Pin *currentPin) {
        auto isCurrentPinInLink = [&](const auto &link) { return currentPin->id == link->pinIds.first || currentPin->id == link->pinIds.second; };
        auto linksWithThisPin   = links | std::views::filter(isCurrentPinInLink);

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

std::vector<Link *> Logic::getPinLinks(const Pin *pin) const
{
    return links | ranges::views::transform(ToNonOwningPointer()) |
           ranges::views::filter([pin](const Link *link) { return pin->id == link->pinIds.first || pin->id == link->pinIds.second; }) |
           ranges::to_vector;
}

bool Logic::arePinsAlreadyConnected(const Pin *first, const Pin *second) const
{
    return ranges::any_of(first->connectedPins, [&](const Pin *connectedPin) { return connectedPin->id == second->id; });
}

bool Logic::arePinsOnSameNode(const Pin *first, const Pin *second) const
{
    return first->parentNode == second->parentNode;
}

bool Logic::isInputPin(const Pin *pin) const
{
    return ranges::any_of(pin->parentNode->inputPins, [&](const std::unique_ptr<Pin> &nodePin) { return nodePin.get() == pin; });
}

const std::unique_ptr<Node> &Logic::getNodeWithPin(IDType pinId) const
{
    return *ranges::find_if(nodes, [&](const std::unique_ptr<Node> &node) {
        return ranges::any_of(getPinsOnNode(node.get()), [&](const Pin *pin) { return pin->id == pinId; });
    });
}

void Logic::cleanNonInputNodesPins()
{
    auto nonInputNodes{nodes | ranges::views::filter([&](std::unique_ptr<Node> &node) { return !isNodeAnInput(node.get()); })};

    std::ranges::for_each(nonInputNodes, [](std::unique_ptr<Node> &node) {
        std::ranges::for_each(node->inputPins, [](std::unique_ptr<Pin> &pin) { pin->payload.reset(); });
        std::ranges::for_each(node->outputPins, [](std::unique_ptr<Pin> &pin) { pin->payload.reset(); });
    });
}

void Logic::clearAll()
{
    links.clear();
    nodes.clear();
    idProvider.reset();
}

const Link *Logic::getLinkById(IDType linkId) const
{
    auto hasGivenId    = [linkId](const auto &link) { return link->id == linkId; };
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
