#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/ImageSaveNode/ImageSaveNode.hpp"

#include "Application/TexturesOperationsProxySingleton.hpp"

struct ImageSaveNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    ImageSaveNodeDrawStrategy(ImageSaveNode *imageSaveNode);

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    ImageSaveNode *imageSaveNode;
};