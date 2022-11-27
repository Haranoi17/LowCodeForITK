#include "Node.hpp"
#include <algorithm>

void Node::acceptDrawVisitor(DrawVisitor *drawVisitor)
{
    drawVisitor->drawNode(this);
}
