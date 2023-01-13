#pragma once
#include "DrawStrategy\Interface\DrawStrategy.hpp"
#include "Nodes\Interface\Node.hpp"
#include <application.h>

struct NodeDrawStrategy : public DrawStrategy
{
    NodeDrawStrategy(Node *node);

    void draw() override;

    const IDType nodeToDrawID;

  protected:
    Node *m_node;
};