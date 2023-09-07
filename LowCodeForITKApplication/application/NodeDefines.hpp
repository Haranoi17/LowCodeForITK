#pragma once

#include "Application/Drawing/DrawStrategies/BinaryThresholdNodeDrawStrategy/BinaryThresholdNodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Application/Drawing/DrawStrategies/EdgeDetectionNodeDrawStrategy/EdgeDetectionNodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/FloatingPointValueNodeDrawStrategy/FloatingPointValueNodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/GaussianBlurNodeDrawStrategy/GaussianBlurNodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/ImageReadNodeDrawStrategy/ImageReadNodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/ImageSaveNodeDrawStrategy/ImageSaveNodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/ImageViewNodeDrawStrategy/ImageViewNodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/PercentageNodeDrawStrategy/PercentageNodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/RGBANodeDrawStrategy/RGBANodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/RegistrationNodeDrawStrategy/RegistrationNodeDrawStrategy.hpp"
#include "Application/Drawing/DrawStrategies/TintNodeDrawStrategy/TintNodeDrawStrategy.hpp"

#include "Logic/Nodes/BinaryThresholdNode/BinaryThresholdNode.hpp"
#include "Logic/Nodes/EdgeDetectionNode/EdgeDetectionNode.hpp"
#include "Logic/Nodes/FloatingPointValueNode/FloatingPointValueNode.hpp"
#include "Logic/Nodes/GaussianBlurNode/GaussianBlurNode.hpp"
#include "Logic/Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Logic/Nodes/ImageSaveNode/ImageSaveNode.hpp"
#include "Logic/Nodes/ImageViewNode/ImageViewNode.hpp"
#include "Logic/Nodes/PercentageNode/PercentageNode.hpp"
#include "Logic/Nodes/RGBANode/RGBANode.hpp"
#include "Logic/Nodes/RegistrationNode/RegistrationNode.hpp"
#include "Logic/Nodes/TintNode/TintNode.hpp"

#include "Logic/UniqueIDProvider/SimpleIDProvider/SimpleIDProvider.hpp"

#include "Application/Drawing/TextureOperator.hpp"
#include "Application/Drawing/TexturesRepository.hpp"
#include "Logic/NodeCreator.hpp"
#include "Logic/NodesRepository.hpp"

#include <concepts>


template <NodeDerived NodeTypeParam, typename DrawStrategyTypeParam, Functionality functionalityParam>
struct Implementation
{
    using NodeType         = NodeTypeParam;
    using DrawStrategyType = DrawStrategyTypeParam;

    static constexpr Functionality functionality{functionalityParam};
};

using DrawStrategyCreator = std::function<std::unique_ptr<NodeDrawStrategy>(Node *, TextureOperator *, TexturesRepository *, ColorPicker *)>;

template <typename T>
concept NodeImplementation = requires() {
                                 T::NodeType;
                                 T::DrawStrategyType;
                                 T::functionality;
                             };

template <NodeImplementation... Implementations>
struct NodesRepositoryVariadic : public NodesDefinitions
{
    using NodesDefinitions::NodeNameType;

    std::vector<NodeNameType> getRegisteredNodeTypeNames() override
    {
        std::vector<NodeNameType> typeNamesVec{};
        (typeNamesVec.emplace_back(Implementations::NodeType::typeName), ...);
        return typeNamesVec;
    }

    std::map<NodeNameType, Functionality> getMapOfNodeFunctionalities() override
    {
        using pair = std::pair<NodeNameType, Functionality>;
        std::map<NodeNameType, Functionality> map{};

        (map.insert(pair{Implementations::NodeType::typeName, Implementations::functionality}), ...);
        return map;
    }

    std::map<NodeNameType, std::unique_ptr<NodeCreator>> getMapOfNodeCreators() override
    {
        using pair = std::pair<NodeNameType, std::unique_ptr<NodeCreator>>;
        auto map   = std::map<NodeNameType, std::unique_ptr<NodeCreator>>{};

        (map.insert(pair{Implementations::NodeType::typeName, std::make_unique<SpecificNodeCreator<typename Implementations::NodeType>>()}), ...);
        return map;
    }

    std::vector<std::unique_ptr<Node>> getMockedNodes() override
    {
        auto vec = std::vector<std::unique_ptr<Node>>{};

        (vec.emplace_back(getMapOfNodeCreators().at(Implementations::NodeType::typeName)->createMocked()), ...);

        return vec;
    }

    std::map<NodesDefinitions::NodeNameType, DrawStrategyCreator> getMapOfDrawStrategyCreators()
    {
        using pair = std::pair<NodeNameType, DrawStrategyCreator>;

        auto map = std::map<NodeNameType, DrawStrategyCreator>{};

        (map.insert(pair{Implementations::NodeType::typeName,
                         [](Node *nodePtr, TextureOperator *textureOperator, TexturesRepository *texturesRepository, ColorPicker *colorPicker) {
                             return std::make_unique<Implementations::DrawStrategyType>(nodePtr, textureOperator, texturesRepository, colorPicker);
                         }}),
         ...);

        return map;
    }
};

using ApplicationNodesRepository =
    NodesRepositoryVariadic<Implementation<RGBANode, RGBANodeDrawStrategy, Functionality::Input>,
                            Implementation<ImageViewNode, ImageViewNodeDrawStrategy, Functionality::Output>,
                            Implementation<GaussianBlurNode, GaussianBlurNodeDrawStrategy, Functionality::Filtering>,
                            Implementation<EdgeDetectionNode, EdgeDetectionNodeDrawStrategy, Functionality::FeatureExtraction>,
                            Implementation<ImageReadNode, ImageReadNodeDrawStrategy, Functionality::Read>,
                            Implementation<FloatingPointValueNode, FloatingPointValueNodeDrawStrategy, Functionality::Input>,
                            Implementation<PercentageNode, PercentageNodeDrawStrategy, Functionality::Input>,
                            Implementation<TintNode, TintNodeDrawStrategy, Functionality::Filtering>,
                            Implementation<ImageSaveNode, ImageSaveNodeDrawStrategy, Functionality::Write>,
                            Implementation<RegistrationNode, RegistrationNodeDrawStrategy, Functionality::Registration>,
                            Implementation<BinaryThresholdNode, BinaryThresholdNodeDrawStrategy, Functionality::Segmentation>>;
