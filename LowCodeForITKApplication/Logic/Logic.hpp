#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "DrawStrategy/NodeDrawStrategy.hpp"
#include "Nodes/Interface/Node.hpp"
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include <functional>
#include <utility>

struct NodeWithDrawStrategy
{
    std::unique_ptr<Node>             node;
    std::unique_ptr<NodeDrawStrategy> drawStrategy;
};

#include <nlohmann\json.hpp>

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

    const std::vector<std::unique_ptr<NodeDrawStrategy>> &getNodesDrawStrategies();
    const std::vector<std::unique_ptr<LinkInfo>>         &getLinks();
    const std::vector<std::unique_ptr<Node>>             &getNodes()
    {
        return m_nodes;
    }

    std::map<std::string, std::function<std::unique_ptr<NodeWithDrawStrategy>()>> m_nodeCreators;

    json serialize() override;

    void deserialize(json data) override;

  private:
    static std::map<std::string, std::function<std::unique_ptr<Node>()>> nodeTypeNameToFactoryMethodMap;
    static std::map<std::string, std::function<std::unique_ptr<NodeDrawStrategy>(Node *)>>
        nodeTypeNameToDrawStrategyMap;

    json serializeLinks();

    json serializeNodes();

    void deserializeLinks(json links);

    void deserializeNodes(json nodes);

    void updatePinsAfterDeserialization();

    std::vector<LinkInfo *> getPinLinks(Pin *pin) const;
    Pin                    *getPinById(IDType id) const;
    Node                   *getNodeById(IDType id) const;

    bool isPinOnNode(const Node *node, const Pin *pin) const;
    bool isNodeAnInput(const Node *node) const;
    bool arePinsOnSameNode(const Pin *first, const Pin *second) const;
    bool areNodesConnected(const Node *first, const Node *second) const;
    bool arePinsAlreadyConnected(const Pin *first, const Pin *second) const;

    bool isInputPin(const Pin *pin) const;

    std::vector<Pin *> collectAllPins() const;
    std::vector<Pin *> getPinsOnNode(const Node *node) const;

    const LinkInfo              *getLinkInfoById(IDType linkId) const;
    const std::unique_ptr<Node> &getNodeWithPin(IDType pinId) const;

    void cleanNonInputNodesPins();

    void clearAll()
    {
        m_links.clear();
        m_nodes.clear();
        m_drawNodeStrategies.clear();
        m_idProvider.reset();
    }

    std::vector<std::unique_ptr<LinkInfo>>         m_links;
    std::vector<std::unique_ptr<Node>>             m_nodes;
    std::vector<std::unique_ptr<NodeDrawStrategy>> m_drawNodeStrategies;
    std::unique_ptr<UniqueIDProvider>              m_idProvider;
};
