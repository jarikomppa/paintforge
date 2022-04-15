//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef WIDGET_H_INCLUDED
#define WIDGET_H_INCLUDED

class Widget
{
public:
    Widget();
    virtual ~Widget();
    virtual void render();
    virtual INT checkHit(INT32 aX, INT32 aY, INT32 aEvent);
    virtual INT checkHit(INT32 aX, INT32 aY, INT32 aEvent, XFcRectangle *aArea);
    Widget *mNext;
    XFcRectangle mHitRegion;
};

#endif // PFWIDGET_H_INCLUDED