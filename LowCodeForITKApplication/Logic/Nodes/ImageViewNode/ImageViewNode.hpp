#pragma once
#include "Logic/Nodes/ImageReadNode/ImageReadNode.hpp"
#include "Logic/Nodes/Node.hpp"
#include "Logic/UniqueIDProvider/UniqueIDProvider.hpp"
#include <imgui.h>
#include <itkImage.h>

#include <future>

class ImageViewNode : public Node
{
  public:
    using PixelType = itk::RGBAPixel<unsigned char>;
    using ImageType = itk::Image<PixelType, 2>;

    inline static std::string typeName = "ImageViewNode";
    ImageViewNode(UniqueIDProvider *idProvider, std::string_view name = typeName);
    ImageViewNode() = default;

    void calculate() override;

    void deserialize(json data) override;

    Pin *imagePin;

    bool                                        imageChanged{false};
    int                                         width{};
    int                                         height{};
    std::unique_ptr<PixelType::ComponentType[]> flatImageArray;
    int                                         previousAllocationSize{};
    std::vector<std::future<void>>              pixelOperations;
};
