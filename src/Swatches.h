//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef SWATCHES_H_INCLUDED
#define SWATCHES_H_INCLUDED

class Swatches : public Widget
{
public:
    Swatches();
    virtual void render();
    virtual INT checkHit(INT32 aX, INT32 aY, INT32 aEvent);
};

#endif // PFWIDGET_H_INCLUDED