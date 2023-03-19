#pragma once
#include "Logic/Interfaces/Serializeable.hpp"
#include "Logic/UniqueIDProvider/UniqueIDProvider.hpp"

#include <any>
#include <iostream>
#include <memory>
#include <vector>

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
    }

    json serialize() override;
    void deserialize(json data) override;

    IDType             id;
    Node              *parentNode;
    std::vector<Pin *> connectedPins;
    std::string        name;
    std::any           payload;
};