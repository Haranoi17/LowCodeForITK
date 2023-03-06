#pragma once
#include "Pins/Interface/Pin.hpp"

#define NewPinType(NAME, NAME_STR)                                                             \
    struct NAME : public Pin                                                                   \
    {                                                                                          \
        inline static std::string typeName{NAME_STR};                                          \
        NAME(UniqueIDProvider *idProvider, Node *parentNode, std::string_view name = NAME_STR) \
            : Pin{idProvider, parentNode, name}                                                \
        {                                                                                      \
        }                                                                                      \
        NAME() = default;                                                                      \
    };

NewPinType(ImagePin, "ImagePin");
NewPinType(RGBAPin, "RGBAPin");

inline std::map<std::string, std::function<std::unique_ptr<Pin>()>> typeNameToPinObject{
    {ImagePin::typeName, []() { return std::make_unique<ImagePin>(); }},
    {RGBAPin::typeName, []() { return std::make_unique<RGBAPin>(); }}};