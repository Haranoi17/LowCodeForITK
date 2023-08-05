#include "ImageViewNodeDrawStrategy.hpp"

#include <format>

#include "Application/Drawing/DrawDefaults.hpp"
#include "imgui_node_editor.h"
#include <itkImageIterator.h>

namespace ed = ax::NodeEditor;

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
        textureOperator->AddTextureToDestroy(textureId);
    }
}

void ImageViewNodeDrawStrategy::createNewTexture()
{
    textureId = textureOperator->CreateTexture(imageViewNode->flatImageArray.get(), imageViewNode->width, imageViewNode->height);
}

void ImageViewNodeDrawStrategy::touchNode()
{
    imageViewNode->imageChanged = false;
}

void ImageViewNodeDrawStrategy::drawTexture()
{
    ImGui::Image(textureId, ImVec2{static_cast<float>(imageViewNode->width), static_cast<float>(imageViewNode->height)});
}
