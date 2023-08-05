#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/ImageSaveNode/ImageSaveNode.hpp"

struct ImageSaveNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    ImageSaveNode *imageSaveNode{dynamic_cast<ImageSaveNode*>(node)};
};