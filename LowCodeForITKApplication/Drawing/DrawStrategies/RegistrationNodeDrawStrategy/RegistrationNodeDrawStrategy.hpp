#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/RegistrationNode/RegistrationNode.hpp"

#include "Application/TexturesOperationsProxySingleton.hpp"

struct RegistrationNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    RegistrationNodeDrawStrategy(RegistrationNode *registrationNode);

  private:
    RegistrationNode *registrationNode;
};