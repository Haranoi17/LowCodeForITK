#include "ImageViewNodeDrawStrategy.hpp"

#include <format>

// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image/stb_image.h>

#include "Application/LowCodeForITKApplication.hpp"

#include "Drawing/DrawDefaults.hpp"
#include "imgui_node_editor.h"
#include <itkImageIterator.h>

namespace ed = ax::NodeEditor;

ImageViewNodeDrawStrategy::ImageViewNodeDrawStrategy(ImageViewNode *imageViewNode)
    : BlueprintNodeDrawStrategy{imageViewNode}, imageViewNode{imageViewNode}
{
    texturesOperations = TexturesOperationsProxySingleton::instance();
}

void ImageViewNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    if (imageViewNode->imageChanged)
    {
        destroyPreviousTexture();
        createNewTexture();
        touchNode();
    }

    drawTexture();
}

void ImageViewNodeDrawStrategy::destroyPreviousTexture()
{
    if (textureId)
    {
        texturesOperations->AddTextureToDestroy(textureId);
    }
}

void ImageViewNodeDrawStrategy::createNewTexture()
{
    textureId = texturesOperations->CreateTexture(
        imageViewNode->flatImageArray.get(), imageViewNode->width, imageViewNode->height);
}

void ImageViewNodeDrawStrategy::touchNode()
{
    imageViewNode->imageChanged = false;
}

void ImageViewNodeDrawStrategy::drawTexture()
{
    ImGui::Image(textureId,
                 ImVec2{static_cast<float>(imageViewNode->width), static_cast<float>(imageViewNode->height)});
}
