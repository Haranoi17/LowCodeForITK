#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/ImageViewNode/ImageViewNode.hpp"

struct ImageViewNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    void destroyPreviousTexture();
    void createNewTexture();
    void touchNode();
    void drawTexture();

    ImageViewNode *imageViewNode{dynamic_cast<ImageViewNode *>(node)};

    ImTextureID textureId{};
};