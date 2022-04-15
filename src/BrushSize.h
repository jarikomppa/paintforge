//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef BRUSHSIZE_H_INCLUDED
#define BRUSHSIZE_H_INCLUDED

class BrushSize : public Widget
{
public:
    BrushSize();
    virtual void render();
    virtual INT checkHit(INT32 aX, INT32 aY, INT32 aEvent);
};

#endif // PFWIDGET_H_INCLUDED