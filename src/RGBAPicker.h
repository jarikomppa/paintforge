//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef RGBAPICKER_H_INCLUDED
#define RGBAPICKER_H_INCLUDED

class RGBAPicker : public Widget
{
public:
    RGBAPicker();
    virtual void render();
    virtual INT checkHit(INT32 aX, INT32 aY, INT32 aEvent);
    INT mPickedControl;
};

#endif // PFWIDGET_H_INCLUDED