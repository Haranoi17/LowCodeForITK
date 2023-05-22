#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Drawing/DrawStrategies/EdgeDetectionNodeDrawStrategy/EdgeDetectionNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/FloatingPoinValueNodeDrawStrategy/FloatingPoinValueNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/GaussianBlurNodeDrawStrategy/GaussianBlurNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/ImageReadNodeDrawStrategy/ImageReadNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/ImageViewNodeDrawStrategy/ImageViewNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/RGBANodeDrawStrategy/RGBANodeDrawStrategy.hpp"

#include "Logic/Nodes/EdgeDetectionNode/EdgeDetectionNode.hpp"
#include "Logic/Nodes/FloatingPoinValueNode/FloatingPoinValueNode.hpp"
#include "Logic/Nodes/GaussianBlurNode/GaussianBlurNode.hpp"
#include "Logic/Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Logic/Nodes/ImageViewNode/ImageViewNode.hpp"
#include "Logic/Nodes/RGBANode/RGBANode.hpp"

#include "Logic/UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"

enum class Functionality
{
    Filtering,
    Input,
    Output,
};

class NodeCreator
{
  public:
    virtual ~NodeCreator()                                                            = default;
    virtual std::unique_ptr<Node> createFullyFunctional(UniqueIDProvider *idProvider) = 0;
    virtual std::unique_ptr<Node> createForDeserialization()                          = 0;
    virtual std::unique_ptr<Node> createMocked()                                      = 0;
};

template <typename NodeType>
class SpecificNodeCreator : public NodeCreator
{
  public:
    std::unique_ptr<Node> createFullyFunctional(UniqueIDProvider *idProvider) override
    {
        return std::make_unique<NodeType>(idProvider);
    };

    std::unique_ptr<Node> createForDeserialization() override
    {
        return std::make_unique<NodeType>();
    };

    std::unique_ptr<Node> createMocked() override
    {
        SimpleIDProvider mockIdProvider;
        return std::make_unique<NodeType>(&mockIdProvider);
    };
};

inline const std::map<Functionality, std::string> functionalityNameMap{
    {Functionality::Filtering, "Filtering"}, {Functionality::Input, "Input"}, {Functionality::Output, "Output"}};

#define NODE_TYPE_TO_DRAW_STRATEGY_FACTORY_METHOD(NODETYPE) \
    [](Node *nodePtr) { return std::make_unique<NODETYPE##DrawStrategy>(dynamic_cast<NODETYPE *>(nodePtr)); }

inline std::vector<std::string>                            registeredNodeTypes{};
inline std::map<std::string, Functionality>                nodeTypeFunctionalityMap{};
inline std::map<std::string, std::unique_ptr<NodeCreator>> nodeTypeNameToCreatorMap{};
inline std::map<std::string, std::function<std::unique_ptr<NodeDrawStrategy>(Node *)>>
    nodeTypeNameToDrawStrategyCreatorMap{};

#define REGISTER_NEW_NODE(NODETYPE, FUNCTIONALITY)                                                                \
                                                                                                                  \
    registeredNodeTypes.push_back(NODETYPE::typeName);                                                            \
    nodeTypeFunctionalityMap[NODETYPE::typeName]             = FUNCTIONALITY;                                     \
    nodeTypeNameToCreatorMap[NODETYPE::typeName]             = std::make_unique<SpecificNodeCreator<NODETYPE>>(); \
    nodeTypeNameToDrawStrategyCreatorMap[NODETYPE::typeName] = NODE_TYPE_TO_DRAW_STRATEGY_FACTORY_METHOD(NODETYPE);

inline void RegisterNodes()
{
    REGISTER_NEW_NODE(RGBANode, Functionality::Filtering);
    REGISTER_NEW_NODE(ImageViewNode, Functionality::Output);
    REGISTER_NEW_NODE(GaussianBlurNode, Functionality::Filtering);
    REGISTER_NEW_NODE(EdgeDetectionNode, Functionality::Filtering);
    REGISTER_NEW_NODE(ImageReadNode, Functionality::Input);
    REGISTER_NEW_NODE(FloatingPoinValueNode, Functionality::Input);
    REGISTER_NEW_NODE(PercentageNode, Functionality::Input);
}
