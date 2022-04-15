//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#include <xforge.h>
#include <xfutil/XFuPrinter.h>
#include "PaintForge.h"
#include "Widget.h"
#include "Canvas.h"
#include "BrushSize.h"

BrushSize::BrushSize()
{
    mHitRegion = XFcRectangle(6,289,48*8,4*8);
}

void BrushSize::render()
{
    PaintForge *pf = PaintForge::getInstance();
    XFcGLSurface *fb = pf->mGL->getSecondary();
    Canvas * c = pf->mCanvas;

    fb->drawFilledRect(6 + (c->mBrushSize * 168) / 32,287, 3, 29, 0xffafafaf);
    fb->drawFilledRect(179 + (c->mBrushHardness * 168) / 32,287, 3, 29, 0xffafafaf);
}

INT BrushSize::checkHit(INT32 aX, INT32 aY, INT32 aEvent)
{
    if (Widget::checkHit(aX,aY,aEvent))
    {
        Canvas * c = PaintForge::getInstance()->mCanvas;
        if (aX < 174)
        {
            c->mBrushSize = ((aX - 6) * 32) / 168;
            if (c->mBrushSize < 2) c->mBrushSize = 2;
            if (c->mBrushSize > 32) c->mBrushSize = 32;
        }
        if (aX > 179)
        {
            c->mBrushHardness = ((aX - 179) * 32) / 168;
            if (c->mBrushHardness < 0) c->mBrushHardness = 0;
            if (c->mBrushHardness > 32) c->mBrushHardness = 32;
        }

        c->createBrush();
        return 1;
    }
    return 0;
}
