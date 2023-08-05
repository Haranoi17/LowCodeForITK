#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/RegistrationNode/RegistrationNode.hpp"

struct RegistrationNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

  private:
    RegistrationNode *registrationNode{dynamic_cast<RegistrationNode *>(node)};
};