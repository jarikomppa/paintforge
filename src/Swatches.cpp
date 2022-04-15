//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//
#include <xforge.h>
#include <xfutil/XFuPrinter.h>
#include "PaintForge.h"
#include "Widget.h"
#include "Canvas.h"
#include "Swatches.h"
#include "RGBAPicker.h"


Swatches::Swatches()
{
    mHitRegion = XFcRectangle(18,116,5 * 16,10 * 16);
}

void Swatches::render()
{
    PaintForge *pf = PaintForge::getInstance();
    XFcGLSurface *fb = pf->mGL->getSecondary();

    INT32 i,j;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 10; j++)
        {
            if (i == 4)
            {
                fb->drawFilledRect(18 + i * 16,116 + j * 16,15,15,0xff000000 | pf->mSwatches[i * 10 + j]);
            }
            else
            {
                fb->drawFilledRect(18 + i * 16,116 + j * 16,15,15,0xff000000 | pf->mSwatches[i * 10 + j]);
            }
        }
    }
}


INT Swatches::checkHit(INT32 aX, INT32 aY, INT32 aEvent)
{
    PaintForge *pf = PaintForge::getInstance();
    Canvas *c = pf->mCanvas;
    if (Widget::checkHit(aX, aY, aEvent))
    {        
        c->mColor = pf->mSwatches[((aX - 18) / 16) * 10 + ((aY - 116) / 16)];
        c->mColorHSV = rgb2hsv(c->mColor);
        c->createBrush();
        return 1;
    }
    return 0;
}