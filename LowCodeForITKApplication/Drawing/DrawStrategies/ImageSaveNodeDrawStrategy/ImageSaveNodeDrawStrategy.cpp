#include "ImageSaveNodeDrawStrategy.hpp"
#include "Application/LowCodeForITKApplication.hpp"

ImageSaveNodeDrawStrategy::ImageSaveNodeDrawStrategy(ImageSaveNode *imageSaveNode)
    : BlueprintNodeDrawStrategy{imageSaveNode}, imageSaveNode{imageSaveNode}
{
}

void ImageSaveNodeDrawStrategy::nodeSpecificFunctionalitiesBeforeNodeEnd()
{
    constexpr int size = 100;
    char          fileNameBuffor[100];
    strcpy(fileNameBuffor, imageSaveNode->fileName.c_str());

    ImGui::InputText("Image file name", fileNameBuffor, size);

    imageSaveNode->fileName = fileNameBuffor;

    if (ImGui::Button("save"))
    {
        imageSaveNode->save();
    }
}
