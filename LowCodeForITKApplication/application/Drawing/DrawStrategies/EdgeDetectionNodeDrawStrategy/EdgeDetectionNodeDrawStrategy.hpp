#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/EdgeDetectionNode/EdgeDetectionNode.hpp"

struct EdgeDetectionNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

  private:
    EdgeDetectionNode *edgeDetectionNode{dynamic_cast<EdgeDetectionNode *>(node)};
};