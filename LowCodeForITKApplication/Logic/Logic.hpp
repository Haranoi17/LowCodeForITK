#pragma once

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

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

class Logic
{
  public:
    void init();

    void propagateEvaluationThroughTheNodes();
    void chainReaction(Pin *outputPin);

    void deleteLink(IDType linkId);
    bool isLinkPossible(std::pair<IDType, IDType> pinIdPair);
    void createLink(std::pair<IDType, IDType> pinIdPair);

    void addNode(std::unique_ptr<Node> newNode);

    const std::vector<std::unique_ptr<Node>>     &getNodes();
    const std::vector<std::unique_ptr<LinkInfo>> &getLinks();

    std::map<std::string, std::function<std::unique_ptr<Node>()>> m_nodeCreators;

  private:
    Pin *getPinById(IDType id) const;

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

    std::vector<std::unique_ptr<LinkInfo>> m_links;
    std::vector<std::unique_ptr<Node>>     m_nodes;
    std::unique_ptr<UniqueIDProvider>      m_idProvider;
};