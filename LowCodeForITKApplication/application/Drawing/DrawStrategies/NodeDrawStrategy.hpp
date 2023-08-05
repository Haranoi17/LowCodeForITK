#pragma once
#include "Application/Drawing/DrawStrategy.hpp"
#include "Application/Drawing/TextureOperator.hpp"
#include "Application/Drawing/TexturesRepository.hpp"
#include "Logic/Nodes/Node.hpp"
#include <application.h>

struct NodeDrawStrategy : public DrawStrategy
{
    NodeDrawStrategy(Node *node, TextureOperator* textureOperator, TexturesRepository* textureRepository);

    void draw() override;

    const IDType nodeToDrawID;

  protected:
    Node *node;
    TextureOperator    *textureOperator;
    TexturesRepository *textureRepository;
};