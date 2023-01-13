#include "DrawStrategy\BlueprintNodeDrawStarategy.hpp"
#include "Nodes\ImageViewNode\ImageViewNode.hpp"

struct ImageViewNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    ImageViewNodeDrawStrategy(ImageViewNode *imageViewNode);
    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    ImageViewNode *m_imageViewNode;
};