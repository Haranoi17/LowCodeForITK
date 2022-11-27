#pragma once
#include <memory>
#include <vector>

#include "DrawVisitor/Interface/DrawVisitor.hpp"
#include "Pins/Interface/Pin.hpp"
#include <iostream>

class Node
{
  public:
    Node(UniqueIDProvider *idProvider, std::string_view name = "NodeBase") : id{idProvider->generateID()}, name{name}
    {
    }

    virtual void acceptDrawVisitor(DrawVisitor *drawStrategy);
    virtual void calculate()
    {
        std::cout << "NODE:\n   " << name << " node calculation\n";
    };

    virtual void populateOutputPins()
    {
        std::cout << "  " << name << " populating output pins \n";
    };

    IDType                            id;
    std::string                       name;
    std::vector<std::unique_ptr<Pin>> m_inputPins;
    std::vector<std::unique_ptr<Pin>> m_outputPins;
};
