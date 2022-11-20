#pragma once

#include <vector>
#include <memory>
#include <string>
#include <map>

#include "Interface/Node.hpp"
#include "Identification/UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include <utility>

struct LinkInfo
{
	LinkInfo(IDType id, std::pair<IDType, IDType> pins) : id{ id }, pinIds{ pins }{}
	IDType id;
	std::pair<IDType, IDType> pinIds;
};

class Logic
{
public:
	void init();

	void propagateEvaluationThroughTheNodes();

	bool isLinkPossible(std::pair<IDType, IDType> pinIdPair);
	void createLink(std::pair<IDType, IDType> pinIdPair);
	void deleteLink(IDType linkId);
	void pushResultsToConnectedInputPins(const std::shared_ptr<Pin>& outputPin);

	const std::vector<std::unique_ptr<LinkInfo>>& getLinks();
	const std::vector<std::unique_ptr<Node>>& getNodes();
private:
	std::shared_ptr<Pin> getPinById(IDType id);

	bool isPinOnNode(const std::unique_ptr<Node>& node, IDType pinId);
	std::vector<std::shared_ptr<Pin>> getPinsOnNode(const std::unique_ptr<Node>& node) const;
	std::vector<std::shared_ptr<Pin>> collectAllPins();
	bool isNodeAnInput(const std::unique_ptr<Node>& node) const;
	bool arePinsAlreadyConnected(const std::shared_ptr<Pin>& first, const std::shared_ptr<Pin>& second);
	bool arePinsOnSameNode(std::pair<IDType, IDType> pinIdPair);
	bool isInputPin(IDType pinId) const;
	bool isOutputPin(IDType pinId) const;
	const std::unique_ptr<Node>& getNodeWithPin(IDType pinId);
	const std::unique_ptr<LinkInfo>& getLinkInfoById(IDType linkId);

	bool areNodesConnected(const std::unique_ptr<Node>& first, const std::unique_ptr<Node>& second) const;
	std::vector<std::unique_ptr<LinkInfo>> m_links;
	std::vector<std::unique_ptr<Node>> m_nodes;
	std::unique_ptr<UniqueIDProvider> m_idProvider;
};