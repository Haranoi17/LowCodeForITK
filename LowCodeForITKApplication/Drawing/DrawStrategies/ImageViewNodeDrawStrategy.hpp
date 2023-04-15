#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/ImageViewNode/ImageViewNode.hpp"

#include "Application/TexturesOperationsProxySingleton.hpp"

struct ImageViewNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    ImageViewNodeDrawStrategy(ImageViewNode *imageViewNode);
    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    void destroyPreviousTexture();
    void createNewTexture();
    void touchNode();
    void drawTexture();

    ImageViewNode *imageViewNode;

    ImTextureID textureId{};

    TexturesOperationsProxySingleton *texturesOperations;
};