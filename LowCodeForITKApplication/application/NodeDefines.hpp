#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Drawing/DrawStrategies/EdgeDetectionNodeDrawStrategy/EdgeDetectionNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/FloatingPoinValueNodeDrawStrategy/FloatingPoinValueNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/GaussianBlurNodeDrawStrategy/GaussianBlurNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/ImageReadNodeDrawStrategy/ImageReadNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/ImageSaveNodeDrawStrategy/ImageSaveNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/ImageViewNodeDrawStrategy/ImageViewNodeDrawStrategy.hpp"
#include "Drawing/DrawStrategies/RGBANodeDrawStrategy/RGBANodeDrawStrategy.hpp"

#include "Logic/Nodes/EdgeDetectionNode/EdgeDetectionNode.hpp"
#include "Logic/Nodes/FloatingPoinValueNode/FloatingPoinValueNode.hpp"
#include "Logic/Nodes/GaussianBlurNode/GaussianBlurNode.hpp"
#include "Logic/Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Logic/Nodes/ImageSaveNode/ImageSaveNode.hpp"
#include "Logic/Nodes/ImageViewNode/ImageViewNode.hpp"
#include "Logic/Nodes/RGBANode/RGBANode.hpp"

#include "Logic/UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"

#include <concepts>

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
    ~SpecificNodeCreator() = default;
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

template <typename T>
concept Reflectable = requires() { T::typeName; };

template <typename T>
concept NodeImplementation = requires() {
                                 requires Reflectable<typename T::NodeType>;
                                 T::DrawStrategyType;
                                 T::functionality;
                             };

template <Reflectable NodeTypeParam, typename DrawStrategyTypeParam, Functionality functionalityParam>
struct Implementation
{
    using NodeType         = NodeTypeParam;
    using DrawStrategyType = DrawStrategyTypeParam;

    static constexpr Functionality functionality{functionalityParam};
};

using DrawStrategyCreator = std::function<std::unique_ptr<NodeDrawStrategy>(Node *)>;

template <NodeImplementation... Implementations>
struct NodesGetter
{
    using TypeNameType = decltype(Node::typeName);

    auto getRegisteredNodeTypeNames()
    {
        std::vector<TypeNameType> typeNamesVec{};
        (typeNamesVec.emplace_back(Implementations::NodeType::typeName), ...);
        return typeNamesVec;
    }

    auto getMapOfNodeFunctionalities()
    {
        using pair = std::pair<TypeNameType, Functionality>;
        std::map<TypeNameType, Functionality> map{};

        (map.insert(pair{Implementations::NodeType::typeName, Implementations::functionality}), ...);
        return map;
    }

    auto getMapOfNodeCreators()
    {
        using pair = std::pair<TypeNameType, std::unique_ptr<NodeCreator>>;
        auto map   = std::map<TypeNameType, std::unique_ptr<NodeCreator>>{};

        (map.insert(pair{Implementations::NodeType::typeName, std::make_unique<SpecificNodeCreator<typename Implementations::NodeType>>()}), ...);
        return map;
    }

    auto getMapOfDrawStrategyCreators()
    {
        using pair = std::pair<TypeNameType, DrawStrategyCreator>;

        auto map = std::map<TypeNameType, DrawStrategyCreator>{};

        (map.insert(pair{
             Implementations::NodeType::typeName,
             [](Node *nodePtr) { return std::make_unique<Implementations::DrawStrategyType>(dynamic_cast<Implementations::NodeType *>(nodePtr)); }}),
         ...);

        return map;
    }

    auto getMockedNodes()
    {
        auto vec = std::vector<std::unique_ptr<Node>>{};

        (vec.emplace_back(getMapOfNodeCreators().at(Implementations::NodeType::typeName)->createMocked()), ...);

        return vec;
    }
};

inline NodesGetter<Implementation<RGBANode, RGBANodeDrawStrategy, Functionality::Input>,
                   Implementation<ImageViewNode, ImageViewNodeDrawStrategy, Functionality::Output>,
                   Implementation<GaussianBlurNode, GaussianBlurNodeDrawStrategy, Functionality::Filtering>,
                   Implementation<EdgeDetectionNode, EdgeDetectionNodeDrawStrategy, Functionality::Filtering>,
                   Implementation<ImageReadNode, ImageReadNodeDrawStrategy, Functionality::Input>,
                   Implementation<FloatingPoinValueNode, FloatingPoinValueNodeDrawStrategy, Functionality::Input>,
                   Implementation<PercentageNode, PercentageNodeDrawStrategy, Functionality::Input>,
                   Implementation<TintNode, TintNodeDrawStrategy, Functionality::Filtering>,
                   Implementation<ImageSaveNode, ImageSaveNodeDrawStrategy, Functionality::Output>>
    nodesGetter;

inline auto registeredNodeTypes{nodesGetter.getRegisteredNodeTypeNames()};
inline auto nodeTypeFunctionalityMap{nodesGetter.getMapOfNodeFunctionalities()};
inline auto nodeTypeNameToCreatorMap{nodesGetter.getMapOfNodeCreators()};
inline auto nodeTypeNameToDrawStrategyCreatorMap{nodesGetter.getMapOfDrawStrategyCreators()};
inline auto mockedNodes{nodesGetter.getMockedNodes()};