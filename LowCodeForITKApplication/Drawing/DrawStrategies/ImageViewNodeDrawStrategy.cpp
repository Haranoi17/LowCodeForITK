#include "ImageViewNodeDrawStrategy.hpp"

#include <format>

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image/stb_image.h>

#include "Application/LowCodeForITKApplication.hpp"
#include "Application/TexturesOperationsProxySingleton.hpp"

#include "Drawing/DrawDefaults.hpp"
#include "imgui_node_editor.h"
#include <itkImageIterator.h>

namespace ed = ax::NodeEditor;

ImageViewNodeDrawStrategy::ImageViewNodeDrawStrategy(ImageViewNode *imageViewNode)
    : BlueprintNodeDrawStrategy{imageViewNode}, imageViewNode{imageViewNode}
{
}

void ImageViewNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    if (textureId)
    {
        ImGui::Image(textureId,
                     ImVec2{static_cast<float>(imageViewNode->width), static_cast<float>(imageViewNode->height)});
    }

    if (!node->dirty)
    {
        return;
    }

    auto textureOperations = TexturesOperationsProxySingleton::instance();

    if (textureId)
    {
        textureOperations->DestroyTexture(textureId);
        textureId = nullptr;
    }

    textureId = textureOperations->CreateTexture(
        imageViewNode->flatImageArray.get(), imageViewNode->width, imageViewNode->height);
}
