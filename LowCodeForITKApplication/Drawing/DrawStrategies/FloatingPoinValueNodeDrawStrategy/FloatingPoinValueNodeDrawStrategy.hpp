#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/FloatingPoinValueNode/FloatingPoinValueNode.hpp"

#include "Application/TexturesOperationsProxySingleton.hpp"

struct FloatingPoinValueNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    FloatingPoinValueNodeDrawStrategy(FloatingPoinValueNode *floatingPoinValueNode);

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    FloatingPoinValueNode *floatingPoinValueNode;
};