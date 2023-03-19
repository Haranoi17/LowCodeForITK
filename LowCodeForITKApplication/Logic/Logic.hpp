#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "Drawing/DrawStrategies/NodeDrawStrategy.hpp"
#include "Logic/Links/Link.hpp"
#include "Logic/Nodes/Node.hpp"
#include "Logic/UniqueIDProvider/UniqueIDProvider.hpp"

#include <nlohmann\json.hpp>

struct NodeWithDrawStrategy
{
    std::unique_ptr<Node>             node;
    std::unique_ptr<NodeDrawStrategy> drawStrategy;
};

class Logic : public Serializable
{
  public:
    Logic();

    void updateCreators();

    void propagateEvaluationThroughTheNodes();
    void chainReaction(Pin *outputPin);

    void deleteLink(IDType linkId);
    void deleteNode(IDType nodeId);
    bool isLinkPossible(std::pair<IDType, IDType> pinIdPair);
    void createLink(std::pair<IDType, IDType> pinIdPair);

    void addNode(std::unique_ptr<Node> newNode);
    void addNodeDrawStrategy(std::unique_ptr<NodeDrawStrategy> nodeDrawStrategy);

    bool innerNodesStateChanged() const;
    void removeDirtyFlagsFromNodes();

    std::vector<NodeDrawStrategy *> getNodesDrawStrategies() const;
    std::vector<LinkInfo *>         getLinks() const;
    std::vector<Node *>             getNodes() const;

    std::map<std::string, std::function<std::unique_ptr<NodeWithDrawStrategy>()>> m_nodeCreators;

    json serialize() override;

    void deserialize(json data) override;

  private:
    static std::map<std::string, std::function<std::unique_ptr<Node>()>> nodeTypeNameToFactoryMethodMap;
    static std::map<std::string, std::function<std::unique_ptr<NodeDrawStrategy>(Node *)>>
        nodeTypeNameToDrawStrategyMap;

    json serializeLinks();

    json serializeNodes();

    void deserializeLinks(json serializedLinks);

    void deserializeNodes(json serializedNodes);

    void updatePinsAfterDeserialization();

    std::vector<LinkInfo *> getPinLinks(const Pin *pin) const;
    Pin                    *getPinById(IDType id) const;
    Node                   *getNodeById(IDType id) const;

    bool isNodeAnInput(const Node *node) const;
    bool arePinsOnSameNode(const Pin *first, const Pin *second) const;
    bool arePinsAlreadyConnected(const Pin *first, const Pin *second) const;

    bool isInputPin(const Pin *pin) const;

    std::vector<Pin *> collectAllPins() const;
    std::vector<Pin *> getPinsOnNode(const Node *node) const;

    const LinkInfo              *getLinkInfoById(IDType linkId) const;
    const std::unique_ptr<Node> &getNodeWithPin(IDType pinId) const;

    void cleanNonInputNodesPins();

    void clearAll();

    std::vector<std::unique_ptr<LinkInfo>>         links;
    std::vector<std::unique_ptr<Node>>             nodes;
    std::vector<std::unique_ptr<NodeDrawStrategy>> nodeDrawStrategies;
    std::unique_ptr<UniqueIDProvider>              idProvider;
};
