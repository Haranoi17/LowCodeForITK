#include "BinaryThresholdNodeDrawStrategy.hpp"
#include "Application/LowCodeForITKApplication.hpp"

BinaryThresholdNodeDrawStrategy::BinaryThresholdNodeDrawStrategy(BinaryThresholdNode *binaryThresholdNode)
    : BlueprintNodeDrawStrategy{binaryThresholdNode}, binaryThresholdNode{binaryThresholdNode}
{
}
