#pragma once
#include "Logic/Pins/Pin.hpp"
#include <optional>

#define NewPinType(NAME)                                                                                                         \
    struct NAME : public Pin                                                                                                     \
    {                                                                                                                            \
        inline static std::string typeName{#NAME};                                                                               \
        NAME(UniqueIDProvider *idProvider, Node *parentNode, std::optional<std::string_view> optionalContextName = std::nullopt, \
             std::string_view typeName = #NAME)                                                                                  \
            : Pin{idProvider, parentNode, optionalContextName, typeName}                                                         \
        {                                                                                                                        \
        }                                                                                                                        \
        NAME() = default;                                                                                                        \
    };

NewPinType(ImagePin);
NewPinType(RGBAPin);
NewPinType(PercentagePin);
NewPinType(FloatingPointValuePin);

inline std::map<std::string, std::function<std::unique_ptr<Pin>()>> typeNameToPinObject{
    {ImagePin::typeName, []() { return std::make_unique<ImagePin>(); }},
    {RGBAPin::typeName, []() { return std::make_unique<RGBAPin>(); }},
    {PercentagePin::typeName, []() { return std::make_unique<PercentagePin>(); }},
    {FloatingPointValuePin::typeName, []() { return std::make_unique<FloatingPointValuePin>(); }},
};