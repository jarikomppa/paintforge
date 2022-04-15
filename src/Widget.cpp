//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#include <xforge.h>
#include "PaintForge.h"
#include "Widget.h"

Widget::Widget()
{
    mNext = NULL;
}

Widget::~Widget()
{
}

void Widget::render()
{
}

INT Widget::checkHit(INT32 aX, INT32 aY, INT32 aEvent, XFcRectangle *aArea)
{
    if (aArea->mX < aX && 
        aArea->mY < aY &&
        aArea->mX + aArea->mWidth >= aX &&
        aArea->mY + aArea->mHeight >= aY)
        return 1;
    return 0;
}


INT Widget::checkHit(INT32 aX, INT32 aY, INT32 aEvent)
{
    if (checkHit(aX, aY, aEvent, &mHitRegion))
        return 1;
    return 0;
}
