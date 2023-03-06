#include "DrawStrategy/BlueprintNodeDrawStarategy.hpp"
#include "Nodes/RGBANode/RGBANode.hpp"

struct RGBANodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    RGBANodeDrawStrategy(RGBANode *rgbaNode);

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    RGBANode *m_rgbaNode;
};