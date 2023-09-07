#pragma once
#include "Logic/Interfaces/Serializeable.hpp"
#include "Logic/UniqueIDProvider/UniqueIDProvider.hpp"

#include <any>
#include <iostream>
#include <memory>
#include <optional>
#include <vector>

class Node;

class Pin : public Serializable
{
  public:
    Pin(UniqueIDProvider *idProvider, Node *parentNode, std::optional<std::string_view> contextName, std::string_view typeName = "anyValue")
        : idProvider{idProvider}, id{idProvider->generateID()}, parentNode{parentNode}, contextName{contextName}, typeName{typeName.data()}
    {
    }

    Pin() = default;
    ~Pin();

    virtual void calculate();

    json serialize() override;
    void deserialize(json data) override;

    std::string getDrawText() const;

    UniqueIDProvider          *idProvider;
    IDType                     id;
    Node                      *parentNode;
    std::vector<Pin *>         connectedPins;
    std::string                typeName;
    std::optional<std::string> contextName;
    std::any                   payload;
};

template <typename T>
concept PinDerived = std::derived_from<T, Pin>;