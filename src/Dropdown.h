//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef DROPDOWN_H_INCLUDED
#define DROPDOWN_H_INCLUDED

class Dropdown : public Widget
{
public:
    Dropdown();
    virtual void render();
    virtual INT checkHit(INT32 aX, INT32 aY, INT32 aEvent);
    INT32 mActiveMenu;
    INT32 mActiveMenuItem;
};

#endif // PFWIDGET_H_INCLUDED