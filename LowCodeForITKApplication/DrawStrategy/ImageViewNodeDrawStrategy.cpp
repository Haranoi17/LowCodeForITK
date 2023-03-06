#include "ImageViewNodeDrawStrategy.hpp"

#include <format>
#include <itkImageFileWriter.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "LowCodeForITKApplication.hpp"
#include "TexturesOperationsProxySingleton.hpp"

#include "DrawDefaults.hpp"
#include "imgui_node_editor.h"
namespace ed = ax::NodeEditor;

ImageViewNodeDrawStrategy::ImageViewNodeDrawStrategy(ImageViewNode *imageViewNode)
    : BlueprintNodeDrawStrategy{imageViewNode}, m_imageViewNode{imageViewNode}
{
}

void ImageViewNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    auto textureOperations = TexturesOperationsProxySingleton::instance();

    if (m_imageViewNode->imagePin->payload.has_value())
    {
        if (!m_imageViewNode->texture || m_node->dirty)
        {
            std::string fileName{std::format("ImageViewNode{}Preview.png", m_imageViewNode->id)};
            auto image = std::any_cast<itk::SmartPointer<ImageViewNode::ImageType>>(m_imageViewNode->imagePin->payload);
            auto writer = itk::ImageFileWriter<ImageViewNode::ImageType>::New();
            writer->SetFileName(fileName);
            writer->SetInput(image);
            writer->Update();

            int component = 0;
            if (auto data =
                    stbi_load(fileName.c_str(), &m_imageViewNode->width, &m_imageViewNode->height, &component, 4))
            {
                m_imageViewNode->texture =
                    textureOperations->CreateTexture(data, m_imageViewNode->width, m_imageViewNode->height);
            }
            m_node->dirty = false;
        }
        else
        {
            ImGui::Image((void *)m_imageViewNode->texture, ImVec2(m_imageViewNode->width, m_imageViewNode->height));
        }
    }
    else
    {
        if (m_imageViewNode->texture)
        {
            textureOperations->DestroyTexture(m_imageViewNode->texture);
            m_imageViewNode->texture = nullptr;
        }
    }
}
