#pragma once
#include <Logic/NodesRepository.hpp>
#include <imgui.h>

class ColorPicker
{
  public:
    ColorPicker(NodesDefinitions *nodesDefinitions) : nodesDefinitions{nodesDefinitions}
    {
    }

    ImColor getColorForNodeType(decltype(Node::typeName) nodeTypeName)
    {
        auto nodeFunctionality = nodesDefinitions->getMapOfNodeFunctionalities().at(nodeTypeName);
        return functionalityColorMap.at(nodeFunctionality);
    }

  private:
    inline static std ::map<Functionality, ImColor> functionalityColorMap{{Functionality::Filtering, ImColor{0.0f, 1.0f, 0.0f}},
                                                                          {Functionality::Input, ImColor{0.0f, 1.0f, 0.8f}},
                                                                          {Functionality::FeatureExtraction, ImColor{0.8f, 0.8f, 0.5f}},
                                                                          {Functionality::Registration, ImColor{0.3f, 0.6f, 0.8f}},
                                                                          {Functionality::Segmentation, ImColor{0.3f, 0.3f, 0.0f}},
                                                                          {Functionality::Write, ImColor{1.0f, 0.5f, 0.0f}},
                                                                          {Functionality::Read, ImColor{0.0f, 0.3f, 0.7f}},
                                                                          {Functionality::Output, ImColor{1.0f, 0.0f, 0.0f}}};
    NodesDefinitions                               *nodesDefinitions;
};
