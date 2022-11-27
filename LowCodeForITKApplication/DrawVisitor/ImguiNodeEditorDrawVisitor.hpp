#pragma once
#include "DrawVisitor/Interface/DrawVisitor.hpp"
#include <application.h>

struct ImguiNodeEditorDrawVisitor : DrawVisitor
{
    ImguiNodeEditorDrawVisitor(Application *app) : imguiApplication{app} {};
    void drawNode(const Node *node) const override;
    void drawInputPin(const Pin *pin) const override;
    void drawOutputPin(const Pin *pin) const override;
    void drawReadImageNode(ReadImageNode *node) const override;
    void drawImageViewNode(const ImageViewNode *node) const override;

  private:
    void defaultNodeBegin(const Node *node) const;

    Application *imguiApplication;
};