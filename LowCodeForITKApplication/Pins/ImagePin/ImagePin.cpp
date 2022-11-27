#include "ImagePin.hpp"

ImagePin::ImagePin(UniqueIDProvider *idProvider, Node *parentNode, std::string_view name)
    : Pin{idProvider, parentNode, name}
{
}