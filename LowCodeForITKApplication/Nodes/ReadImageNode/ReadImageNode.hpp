#pragma once
#include "Nodes/Interface/Node.hpp"
#include <itkImage.h>
#include <itkRGBAPixel.h>

class ReadImageNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    ReadImageNode(UniqueIDProvider *idProvider, std::string_view name = "ReadImage");

    void acceptDrawVisitor(DrawVisitor *drawVisitor) override;
    void populateOutputPins() override;

    std::string imagePath;

  private:
    Pin *imagePin;
};
