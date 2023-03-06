#pragma once
#include <memory>
#include <vector>

#include "Pins/Interface/Pin.hpp"
#include "Serializeable.hpp"

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
    std::vector<std::unique_ptr<Pin>> m_inputPins;
    std::vector<std::unique_ptr<Pin>> m_outputPins;
    bool                              dirty{true};

  private:
    std::vector<std::unique_ptr<Pin>> deserializePins(json pinsData);
};
