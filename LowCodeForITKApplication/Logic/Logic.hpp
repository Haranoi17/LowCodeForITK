#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "DrawStrategy/NodeDrawStrategy.hpp"
#include "Nodes/Interface/Node.hpp"
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include <utility>
struct LinkInfo
{
    LinkInfo(IDType id, std::pair<IDType, IDType> pins) : id{id}, pinIds{pins}
    {
    }
    IDType                    id;
    std::pair<IDType, IDType> pinIds;
};

struct NodeWithDrawStrategy
{
    std::unique_ptr<Node>             node;
    std::unique_ptr<NodeDrawStrategy> drawStrategy;
};

class Logic
{
  public:
    void init();

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

    std::map<std::string, std::function<std::unique_ptr<NodeWithDrawStrategy>()>> m_nodeCreators;

  private:
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

    std::vector<std::unique_ptr<LinkInfo>>         m_links;
    std::vector<std::unique_ptr<Node>>             m_nodes;
    std::vector<std::unique_ptr<NodeDrawStrategy>> m_drawNodeStrategies;
    std::unique_ptr<UniqueIDProvider>              m_idProvider;
};