#pragma once
#include "UniqueIDProvider/Interface/UniqueIDProvider.hpp"
#include <memory>
#include <vector>

#include <Serializeable.hpp>
#include <any>
#include <iostream>

class Node;

class Pin : public Serializable
{
  public:
    Pin(UniqueIDProvider *idProvider, Node *parentNode, std::string_view name = "anyValue")
        : id{idProvider->generateID()}, parentNode{parentNode}, name{name.data()}
    {
    }

    Pin() = default;

    virtual void calculate()
    {
        std::cout << "PIN:\n    " << name << " calculating pin\n";
    }

    json serialize() override;
    void deserialize(json data) override;

    IDType             id;
    Node              *parentNode;
    std::vector<Pin *> connectedPins;
    std::string        name;
    std::any           payload;
};