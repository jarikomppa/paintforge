//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//


#ifndef PFDEADZONE_H_INCLUDED
#define PFDEADZONE_H_INCLUDED

#include "Widget.h"

class DeadZone : public Widget
{
public:
    DeadZone(XFcRectangle aRect, UINT32 aClearColor = 0);
    virtual void render();
    UINT32 mClearColor;
};

#endif //PFDEADZONE_H_INCLUDED