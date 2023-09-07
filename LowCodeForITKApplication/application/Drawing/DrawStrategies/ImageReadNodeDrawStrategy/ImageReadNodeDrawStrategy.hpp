#pragma once

#include "Application/Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/ImageReadNode/ImageReadNode.hpp"

#include "imfilebrowser.h"
#include "imgui.h"

struct ImageReadNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    using BlueprintNodeDrawStrategy::BlueprintNodeDrawStrategy;

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;
    void nodeSpecificFunctionalitiesAfterNodeEnd() override;

  private:
    ImageReadNode *imageViewNode{dynamic_cast<ImageReadNode *>(node)};

    ImGui::FileBrowser filebrowser;
};