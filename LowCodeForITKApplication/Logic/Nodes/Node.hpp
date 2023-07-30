#pragma once
#include <memory>
#include <vector>

#include "Logic/Interfaces/Identifiable.hpp"
#include "Logic/Interfaces/Positionable.hpp"
#include "Logic/Interfaces/Serializeable.hpp"

#include "Logic/Pins/Pin.hpp"
#include "Logic/UniqueIDProvider/UniqueIDProvider.hpp"

class Node : public Serializable, public Positionable
{
  public:
    Node(UniqueIDProvider *idProvider, std::string_view typeName = "NodeBase")
        : idProvider{idProvider}, id{idProvider->generateID()}, typeName{typeName}
    {
    }
    Node() = default;
    ~Node();

    virtual void calculate();
    virtual void populateOutputPins();

    void     setPosition(Position pos) override;
    Position getPosition() const override;

    json serialize() override;
    void deserialize(json data) override;

    UniqueIDProvider                 *idProvider;
    IDType                            id;
    std::string                       typeName;
    std::vector<std::unique_ptr<Pin>> inputPins;
    std::vector<std::unique_ptr<Pin>> outputPins;
    bool                              dirty{true};

  private:
    Position                          position{};
    std::vector<std::unique_ptr<Pin>> deserializePins(json pinsData);
};
