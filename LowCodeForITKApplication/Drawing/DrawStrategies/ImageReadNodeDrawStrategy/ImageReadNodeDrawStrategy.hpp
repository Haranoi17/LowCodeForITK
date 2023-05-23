#include "Drawing/DrawStrategies/BlueprintNodeDrawStarategy.hpp"
#include "Logic/Nodes/ImageReadNode/ImageReadNode.hpp"

#include "imfilebrowser.h"
#include "imgui.h"

struct ImageReadNodeDrawStrategy : public BlueprintNodeDrawStrategy
{
    ImageReadNodeDrawStrategy(ImageReadNode *imageViewNode);

    void nodeSpecificFunctionalitiesBeforeNodeEnd() override;
    void nodeSpecificFunctionalitiesAfterNodeEnd() override;

  private:
    ImageReadNode *m_imageViewNode;

    ImGui::FileBrowser m_filebrowser;
    std::string        previousPath;
};