#pragma once
#include <memory>
#include <vector>

#include "Logic/Interfaces/Identifiable.hpp"
#include "Logic/Interfaces/Serializeable.hpp"
#include "Logic/Pins/Pin.hpp"
#include "Logic/UniqueIDProvider/UniqueIDProvider.hpp"

class Node : public Serializable
{
  public:
    Node(UniqueIDProvider *idProvider, std::string_view name = "NodeBase") : id{idProvider->generateID()}, name{name}
    {
    }

    Node() = default;

    virtual void calculate();
    virtual void populateOutputPins();

    json serialize() override;
    void deserialize(json data) override;

    IDType                            id;
    std::string                       name;
    std::vector<std::unique_ptr<Pin>> inputPins;
    std::vector<std::unique_ptr<Pin>> outputPins;
    bool                              dirty{true};

  private:
    std::vector<std::unique_ptr<Pin>> deserializePins(json pinsData);
};
