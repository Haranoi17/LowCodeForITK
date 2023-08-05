#pragma once

#include "Nodes/Node.hpp"
#include "NodeCreator.hpp"

enum class Functionality
{
    Filtering,
    Registration,
    Segmentation,
    FeatureExtraction,
    Input,
    Output,
    Read,
    Write,
};

const std::map<Functionality, std::string> functionalityNameMap{{Functionality::Filtering, "Filtering"},
                                                                {Functionality::Input, "Input"},
                                                                {Functionality::Output, "Output"},
                                                                {Functionality::Registration, "Registration"},
                                                                {Functionality::Segmentation, "Segmentation"},
                                                                {Functionality::FeatureExtraction, "FeatureExtraction"},
                                                                {Functionality::Read, "Read"},
                                                                {Functionality::Write, "Write"}};


struct NodesDefinitions
{
    using NodeNameType = decltype(Node::typeName);

    ~NodesDefinitions() = default;

    virtual std::vector<NodeNameType>                            getRegisteredNodeTypeNames()   = 0;
    virtual std::map<NodeNameType, Functionality>                getMapOfNodeFunctionalities()  = 0;
    virtual std::map<NodeNameType, std::unique_ptr<NodeCreator>> getMapOfNodeCreators()         = 0;
    virtual std::vector<std::unique_ptr<Node>>                   getMockedNodes()               = 0;
};
