#include "DrawStrategy/NodeDrawStrategy.hpp"
#include "Nodes/ImageReadNode/ImageReadNode.hpp"

struct ImageReadNodeDrawStrategy : public NodeDrawStrategy
{
    ImageReadNodeDrawStrategy(ImageReadNode *imageViewNode);

    void draw() override;

  private:
    ImageReadNode *m_imageViewNode;
};