#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/RGBANode/RGBANode.hpp"

struct RGBANodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    RGBANodeDrawStrategy(RGBANode *rgbaNode);

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    RGBANode *rgbaNode;
};

struct PercentageNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    PercentageNodeDrawStrategy(PercentageNode *percentageNode);

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;

  private:
    PercentageNode *percentageNode;
};

struct TintNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    TintNodeDrawStrategy(TintNode *tintNode);

  private:
    TintNode *tintNode;
};