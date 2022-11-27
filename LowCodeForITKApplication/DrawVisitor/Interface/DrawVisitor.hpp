#pragma once

class Node;
class Pin;
class ReadImageNode;
class ImageViewNode;

struct DrawVisitor
{
    virtual void drawImageViewNode(const ImageViewNode *node) const = 0;
    virtual void drawReadImageNode(ReadImageNode *node) const       = 0;
    virtual void drawNode(const Node *node) const                   = 0;
    virtual void drawInputPin(const Pin *pin) const                 = 0;
    virtual void drawOutputPin(const Pin *pin) const                = 0;
};
