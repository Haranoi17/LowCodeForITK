#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/GaussianBlurNode/GaussianBlurNode.hpp"

#include "Application/TexturesOperationsProxySingleton.hpp"

struct GaussianBlurNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    GaussianBlurNodeDrawStrategy(GaussianBlurNode *gaussianBlurNode);

  private:
    GaussianBlurNode *gaussianBlurNode;
};