#pragma once
#include "Pins/Interface/Pin.hpp"

class ImagePin : public Pin
{
  public:
    ImagePin(UniqueIDProvider *idProvider, Node *parentNode, std::string_view name = "ImagePin");
};