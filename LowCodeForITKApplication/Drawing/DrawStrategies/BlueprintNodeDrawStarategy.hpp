#pragma once
#include "Drawing/DrawStrategies/NodeDrawStrategy.hpp"
#include "Drawing/blueprints/builders.h"
#include "Logic/Nodes/Node.hpp"

struct BlueprintNodeDrawStrategy : public NodeDrawStrategy
{
    BlueprintNodeDrawStrategy(Node *testNode);

    void draw() override;

  private:
    virtual void setStyleVariables();
    virtual void unsetStyleVariables();
    virtual void drawHeader();
    virtual void drawInputPins();
    virtual void drawOutputPins();
    virtual void nodeSpecificFunctionalitiesBeforeNodeEnd();
    virtual void nodeSpecificFunctionalitiesAfterNodeEnd();
    virtual void pushImguiVariables();

    virtual void synchronizeLogicalNodePositionToEditor();
    virtual void setDeserializedPositionInEditior();

    virtual ImColor getHeaderColor();

    ax::NodeEditor::Utilities::BlueprintNodeBuilder nodeBuilder;
};