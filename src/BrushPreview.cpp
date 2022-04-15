//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#include <xforge.h>
#include <xfutil/XFuPrinter.h>
#include "PaintForge.h"
#include "Widget.h"
#include "Canvas.h"
#include "BrushPreview.h"


BrushPreview::BrushPreview()
{
    mHitRegion = XFcRectangle(11,106,48,48);
}

void BrushPreview::render()
{
    PaintForge *pf = PaintForge::getInstance();
    XFcGLSurface *fb = pf->mGL->getSecondary();

    INT32 i, j;

    // "real" brush preview..
    for (i = 0; i < 9; i++)
        for (j = 0; j < 9; j++)
            fb->drawFilledRect(21 + i * 8,29 + j * 8,8,8,(((i&1)^(j&1))?CHECKER_DARK:CHECKER_LIGHT) | (0xff << 24));

    REAL step = (REAL)(pf->mCanvas->biasedSize()) / (REAL)(8);
    if (step < 1) step = 1;

    if (pf->mCanvas->mBrushMode)
    {
        for (i = 0; i < 12; i++)
            fb->drawImage(pf->mCanvas->mBrush,
                          6 * step - i * step + 21 + 4 + 4 * 8 - pf->mCanvas->mBrush->getWidth() / 2, 
                          (6 * step - i * step) / 2 + 29 + 4 + 4 * 8 - pf->mCanvas->mBrush->getHeight() / 2);
    
        for (i = 0; i < 9; i++)
            for (j = 0; j < 9; j++)
                fb->drawFilledRect(21 + i * 8,29 + j * 8,8,8,(((i&1)^(j&1))?CHECKER_DARK:CHECKER_LIGHT) | ((0xff - pf->mCanvas->biasedAlpha()) << 24));
    }
    else
    {
        for (i = 0; i < 12; i++)
            fb->drawImageBlend(pf->mCanvas->mBrush,
                          6 * step - i * step + 21 + 4 + 4 * 8 - pf->mCanvas->mBrush->getWidth() / 2, 
                          (6 * step - i * step) / 2 + 29 + 4 + 4 * 8 - pf->mCanvas->mBrush->getHeight() / 2, XFCBLEND_ALPHA, pf->mCanvas->biasedAlpha());
    }

    // Color preview
    fb->drawFilledRect(21,29 + 7 * 8,16,16,pf->mCanvas->mColor | 0xff000000);

    // Brush mode
    pf->mPrinter->print(fb,21,29,(pf->mCanvas->mBrushMode)?"A":"P");
}


