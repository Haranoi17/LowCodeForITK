#pragma once
#include "DrawStrategy/NodeDrawStrategy.hpp"
#include "Nodes/Interface/Node.hpp"
#include "blueprints/builders.h"

struct BlueprintNodeDrawStrategy : public NodeDrawStrategy
{
    BlueprintNodeDrawStrategy(Node *testNode);

    void draw() override;

  protected:
    virtual void setStyleVariables();
    virtual void unsetStyleVariables();
    virtual void drawHeader();
    virtual void drawInputPins();
    virtual void drawOutputPins();
    virtual void nodeSpecificFunctionalitiesBeforeNodeEnd();
    virtual void nodeSpecificFunctionalitiesAfterNodeEnd();

    ax::NodeEditor::Utilities::BlueprintNodeBuilder m_nodeBuilder;
};