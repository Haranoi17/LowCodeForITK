#pragma once
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include <memory>
#include <vector>

#include <any>
#include <iostream>

class Node;

class Pin
{
  public:
    Pin(UniqueIDProvider *idProvider, Node *parentNode, std::string_view name = "anyValue")
        : id{idProvider->generateID()}, parentNode{parentNode}, name{name.data()}
    {
    }

    virtual void calculate()
    {
        std::cout << "PIN:\n    " << name << " calculating pin\n";
    }

    IDType             id;
    Node              *parentNode;
    std::vector<Pin *> connectedPins;
    std::string        name;
    std::any           payload;
};