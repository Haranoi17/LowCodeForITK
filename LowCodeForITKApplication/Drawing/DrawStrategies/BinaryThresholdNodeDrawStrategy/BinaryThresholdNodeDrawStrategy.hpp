#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/BinaryThresholdNode/BinaryThresholdNode.hpp"

#include "Application/TexturesOperationsProxySingleton.hpp"

struct BinaryThresholdNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    BinaryThresholdNodeDrawStrategy(BinaryThresholdNode *binaryThresholdNode);

  private:
    BinaryThresholdNode *binaryThresholdNode;
};