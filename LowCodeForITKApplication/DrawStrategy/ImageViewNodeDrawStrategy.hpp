#include "DrawStrategy\NodeDrawStrategy.hpp"
#include "Nodes\ImageViewNode\ImageViewNode.hpp"

struct ImageViewNodeDrawStrategy : public NodeDrawStrategy
{
    ImageViewNodeDrawStrategy(ImageViewNode *imageViewNode);
    void draw() override;

  private:
    ImageViewNode *m_imageViewNode;
};