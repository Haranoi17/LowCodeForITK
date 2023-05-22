#include "GaussianBlurNodeDrawStrategy.hpp"
#include "Application/LowCodeForITKApplication.hpp"

GaussianBlurNodeDrawStrategy::GaussianBlurNodeDrawStrategy(GaussianBlurNode *gaussianBlurNode)
    : BlueprintNodeDrawStrategy{gaussianBlurNode}, gaussianBlurNode{gaussianBlurNode}
{
}
