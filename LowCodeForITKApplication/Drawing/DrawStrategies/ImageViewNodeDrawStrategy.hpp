#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/ImageViewNode/ImageViewNode.hpp"

struct ImageViewNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    ImageViewNodeDrawStrategy(ImageViewNode *imageViewNode);
    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    ImageViewNode *imageViewNode;

    ImTextureID textureId{};
};