#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/EdgeDetectionNode/EdgeDetectionNode.hpp"

#include "Application/TexturesOperationsProxySingleton.hpp"

struct EdgeDetectionNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    EdgeDetectionNodeDrawStrategy(EdgeDetectionNode *edgeDetectionNode);

  private:
    EdgeDetectionNode *edgeDetectionNode;
};