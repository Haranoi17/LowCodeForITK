#pragma once
#include "Nodes/Interface/Node.hpp"
#include <itkImage.h>
#include <itkRGBAPixel.h>

class ImageReadNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    ImageReadNode(UniqueIDProvider *idProvider, std::string_view name = "ReadImage");

    void populateOutputPins() override;

    std::string imagePath;

  private:
    Pin *imagePin;
};
