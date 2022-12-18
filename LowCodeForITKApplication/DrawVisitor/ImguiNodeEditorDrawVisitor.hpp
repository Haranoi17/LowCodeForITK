#pragma once
#include "DrawVisitor/Interface/DrawVisitor.hpp"
#include <application.h>
#include <imgui-filebrowser\imfilebrowser.h>

struct ImguiNodeEditorDrawVisitor : DrawVisitor
{
    ImguiNodeEditorDrawVisitor(Application *app) : imguiApplication{app}
    {
        fileBrowser.SetTitle("file browser");
        fileBrowser.SetTypeFilters({".png", ".jpeg", ".jpg"});
    };

    void drawNode(const Node *node) const override;
    void drawInputPin(const Pin *pin) const override;
    void drawOutputPin(const Pin *pin) const override;
    void drawReadImageNode(ReadImageNode *node) override;
    void drawImageViewNode(ImageViewNode *node) override;

  private:
    void defaultNodeBegin(const Node *node) const;

    Application       *imguiApplication;
    ImGui::FileBrowser fileBrowser;
};