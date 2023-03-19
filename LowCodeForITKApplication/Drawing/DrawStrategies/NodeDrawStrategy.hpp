#pragma once
#include "Drawing/DrawStrategy.hpp"
#include "Logic/Nodes/Node.hpp"
#include <application.h>

struct NodeDrawStrategy : public DrawStrategy
{
    NodeDrawStrategy(Node *node);

    void draw() override;

    const IDType nodeToDrawID;

  protected:
    Node *node;
};