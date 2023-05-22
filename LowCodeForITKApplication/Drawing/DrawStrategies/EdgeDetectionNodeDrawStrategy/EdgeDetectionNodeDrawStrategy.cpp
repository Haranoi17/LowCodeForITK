#include "EdgeDetectionNodeDrawStrategy.hpp"
#include "Application/LowCodeForITKApplication.hpp"

EdgeDetectionNodeDrawStrategy::EdgeDetectionNodeDrawStrategy(EdgeDetectionNode *edgeDetectionNode)
    : BlueprintNodeDrawStrategy{edgeDetectionNode}, edgeDetectionNode{edgeDetectionNode}
{
}
