#pragma once
#include <memory>
#include <vector>

#include "Pins/Interface/Pin.hpp"
#include <iostream>

class Node
{
  public:
    Node(UniqueIDProvider *idProvider, std::string_view name = "NodeBase") : id{idProvider->generateID()}, name{name}
    {
    }

    virtual void calculate();

    virtual void populateOutputPins();

    IDType                            id;
    std::string                       name;
    std::vector<std::unique_ptr<Pin>> m_inputPins;
    std::vector<std::unique_ptr<Pin>> m_outputPins;
};
