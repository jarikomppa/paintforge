//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//
#include <xforge.h>
#include <xfutil/XFuPrinter.h>
#include "PaintForge.h"
#include "Widget.h"
#include "Canvas.h"
#include "RGBAPicker.h"


RGBAPicker::RGBAPicker()
{
    mHitRegion = XFcRectangle(113,26,240,255);
}

void RGBAPicker::render()
{
    PaintForge *pf = PaintForge::getInstance();
    XFcGLSurface *fb = pf->mGL->getSecondary();

    INT32 rgb = pf->mCanvas->mColor;
    INT32 hsv = pf->mCanvas->mColorHSV;
    INT32 alpha = pf->mCanvas->mAlpha;
    
    int bpos = 255 - (((rgb >> 0) & 0xff));
    int gpos = 255 - (((rgb >> 8) & 0xff));
    int rpos = 255 - (((rgb >> 16) & 0xff));

    int vpos = 255 - (((hsv >> 0) & 0xff));
    int spos = 255 - (((hsv >> 8) & 0xff));
    int hpos = 255 - (((hsv >> 16) & 0xff));
    
    int apos = 255 - (alpha);

    fb->drawFilledRect(113,27 + bpos - 1, 29, 3, 0xffafafaf);
    fb->drawFilledRect(145,27 + gpos - 1, 29, 3, 0xffafafaf);
    fb->drawFilledRect(177,27 + rpos - 1, 29, 3, 0xffafafaf);
    
    fb->drawFilledRect(216,27 + vpos - 1, 29, 3, 0xffafafaf);
    fb->drawFilledRect(248,27 + spos - 1, 29, 3, 0xffafafaf);
    fb->drawFilledRect(280,27 + hpos - 1, 29, 3, 0xffafafaf);
    
    fb->drawFilledRect(319,27 + apos - 1, 29, 3, 0xffafafaf);
/*
    INT32 i,j;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 10; j++)
        {
            if (i == 4)
            {
                fb->drawFilledRect(18 + i * 16,116 + j * 16,14,14,0xff000000 | pf->mSwatches[i * 10 + j]);
            }
            else
            {
                fb->drawFilledRect(18 + i * 16,116 + j * 16,14,14,0xff000000 | pf->mSwatches[i * 10 + j]);
            }
        }
    }
*/
}


INT RGBAPicker::checkHit(INT32 aX, INT32 aY, INT32 aEvent)
{
    Canvas *c = PaintForge::getInstance()->mCanvas;
    INT32 i;
    if (Widget::checkHit(aX, aY, aEvent))
    {
        if (aEvent == 1)
        {
            mPickedControl = -1;
            if (Widget::checkHit(aX, aY, aEvent, &XFcRectangle(113,27,32,255))) mPickedControl = 0;
            if (Widget::checkHit(aX, aY, aEvent, &XFcRectangle(145,27,32,255))) mPickedControl = 1;
            if (Widget::checkHit(aX, aY, aEvent, &XFcRectangle(177,27,32,255))) mPickedControl = 2;

            if (Widget::checkHit(aX, aY, aEvent, &XFcRectangle(216,27,32,255))) mPickedControl = 3;
            if (Widget::checkHit(aX, aY, aEvent, &XFcRectangle(248,27,32,255))) mPickedControl = 4;
            if (Widget::checkHit(aX, aY, aEvent, &XFcRectangle(280,27,32,255))) mPickedControl = 5;

            if (Widget::checkHit(aX, aY, aEvent, &XFcRectangle(319,27,32,255))) mPickedControl = 6;            
        }
        i = 0xff - (aY - 27);
        if (i < 0) i = 0;
        if (i > 0xff) i = 0xff;
        switch (mPickedControl)
        {
        case 0:
            c->mColor = (c->mColor & 0xffff00) | i << 0;
            c->mColorHSV = rgb2hsv(c->mColor);
            break;
        case 1:
            c->mColor = (c->mColor & 0xff00ff) | i << 8;
            c->mColorHSV = rgb2hsv(c->mColor);
            break;
        case 2:
            c->mColor = (c->mColor & 0x00ffff) | i << 16;
            c->mColorHSV = rgb2hsv(c->mColor);
            break;
        case 3:
            c->mColorHSV = (c->mColorHSV & 0xffff00) | (i << 0);
            c->mColor = hsv2rgb(c->mColorHSV);
            break;
        case 4:
            c->mColorHSV = (c->mColorHSV & 0xff00ff) | (i << 8);
            c->mColor = hsv2rgb(c->mColorHSV);
            break;
        case 5:
            c->mColorHSV = (c->mColorHSV & 0x00ffff) | (i << 16);
            c->mColor = hsv2rgb(c->mColorHSV);
            break;
        case 6:
            c->mAlpha = i;
            break;
        }
        c->createBrush();
        return 1;
    }
    return 0;
}