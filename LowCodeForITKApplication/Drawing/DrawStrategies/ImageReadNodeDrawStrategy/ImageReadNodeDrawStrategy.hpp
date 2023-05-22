#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/ImageReadNode/ImageReadNode.hpp"

#include "imgui.h"

#include "imgui-filebrowser\imfilebrowser.h"

struct ImageReadNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    ImageReadNodeDrawStrategy(ImageReadNode *imageViewNode);

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;
    void nodeSpecificFunctionalitiesAfterNodeEnd() override;

  private:
    ImageReadNode *m_imageViewNode;

    ImGui::FileBrowser m_filebrowser;
};