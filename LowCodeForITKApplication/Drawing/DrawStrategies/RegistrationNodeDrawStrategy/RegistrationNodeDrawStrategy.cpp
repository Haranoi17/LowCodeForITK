#include "RegistrationNodeDrawStrategy.hpp"
#include "Application/LowCodeForITKApplication.hpp"

RegistrationNodeDrawStrategy::RegistrationNodeDrawStrategy(RegistrationNode *registrationNode)
    : BlueprintNodeDrawStrategy{registrationNode}, registrationNode{registrationNode}
{
}
