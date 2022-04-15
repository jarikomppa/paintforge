//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#include <xforge.h>
#include "PaintForge.h"
#include "Canvas.h"
#include "PaletteBackground.h"

void PaletteBackground::init()
{
    PaintForge *pf = PaintForge::getInstance();
    Canvas * c = pf->mCanvas;
    INT32 i,j;
    mBackground = XFcImageLoader::load("logo.pcx");
    //mBackground = XFcImageLoader::load("toolkit.pcx");
/*
    mBackground = XFcGLSurface::create(480,320);
    for (i = 0; i < 320; i++)
        mBackground->drawFilledRect(0,i,480,1,0xff000000 + (((i) & 4) ? 0xbfbfbf: 0xcfcfcf) );
*/
    // Brush preview
    pf->drawFilledRect(mBackground,16,25,16+80,25+80,0xff000000,0x7fffffff);

    // rgb
    pf->drawFilledRect(mBackground,112,26,142,281,0xff000000,0xffffffff);
    pf->drawFilledRect(mBackground,112+32,26,142+32,281,0xff000000,0xffffffff);
    pf->drawFilledRect(mBackground,112+64,26,142+64,281,0xff000000,0xffffffff);

    // hsv
    pf->drawFilledRect(mBackground,215,26,245,281,0xff000000,0xffffffff);
    pf->drawFilledRect(mBackground,215+32,26,245+32,281,0xff000000,0xffffffff);
    pf->drawFilledRect(mBackground,215+64,26,245+64,281,0xff000000,0xffffffff);


    // alpha
    pf->drawFilledRect(mBackground,318,26,348,281,0xff000000,0xffffffff);

    // brush size
    pf->drawFilledRect(mBackground,4,286,172 + 4,30 + 286, 0xff000000, 0x7fffffff);

    // brush sharpness
    pf->drawFilledRect(mBackground,177,286,172 + 177,30 + 286, 0xff000000, 0x7fffffff);

    // layers
    pf->drawFilledRect(mBackground,383,64*0,479,64*1-1, 0xff000000, 0xbfffffff);
    pf->drawFilledRect(mBackground,383,64*1,479,64*2-1, 0xff000000, 0xbfffffff);
    pf->drawFilledRect(mBackground,383,64*2,479,64*3-1, 0xff000000, 0xbfffffff);
    pf->drawFilledRect(mBackground,383,64*3,479,64*4-1, 0xff000000, 0xbfffffff);
    pf->drawFilledRect(mBackground,383,64*4,479,64*5-1, 0xff000000, 0xbfffffff);

    pf->drawFilledRect(mBackground,355,64*0,381,64*1-1, 0xff000000, 0xbfffffff);
    pf->drawFilledRect(mBackground,355,64*1,381,64*2-1, 0xff000000, 0xbfffffff);
    pf->drawFilledRect(mBackground,355,64*2,381,64*3-1, 0xff000000, 0xbfffffff);
    pf->drawFilledRect(mBackground,355,64*3,381,64*4-1, 0xff000000, 0xbfffffff);
    pf->drawFilledRect(mBackground,355,64*4,381,64*5-1, 0xff000000, 0xbfffffff);

    for (i = 0; i < 31; i++)
        for (j = 0; j < 3; j++)
            mBackground->drawFilledRect(319 + j * 8 + 2, 
                                        27 + i * 8 + 4,
                                        8,8,((i&1)^(j&1))?0xffafafef:0xffffffff);



    for (i = 0; i < 254; i++)
    {
        mBackground->drawFilledRect(113,27 + i, 29, 1, 0xff000000 | (((254 - i)) <<  0)); // b
        mBackground->drawFilledRect(145,27 + i, 29, 1, 0xff000000 | (((254 - i)) <<  8)); // g
        mBackground->drawFilledRect(177,27 + i, 29, 1, 0xff000000 | (((254 - i)) << 16)); // r

        mBackground->drawFilledRect(216,27 + i, 29, 1, 0xff000000 | hsv2rgb((((254 - i)) <<  0)|0x000000)); // v
        mBackground->drawFilledRect(248,27 + i, 29, 1, 0xff000000 | hsv2rgb((((254 - i)) <<  8)|0x0000ff)); // s
        mBackground->drawFilledRect(280,27 + i, 29, 1, 0xff000000 | hsv2rgb((((254 - i)) << 16)|0x00ffff)); // h

        mBackground->drawFilledRect(319,27 + i, 29, 1, 0x00000000 | (((254 - i)) << 24)); // a
    }

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 24; j++)
        {
            mBackground->drawFilledRect(6 + j * 7, 289 + i * 8,7,8,((i&1)^(j&1))?0xff7f7f7f:0xff8f8f8f);
            mBackground->drawFilledRect(179 + j * 7, 289 + i * 8,7,8,((i&1)^(j&1))?0xff7f7f7f:0xff8f8f8f);
        }
    }

    c->mBrushHardness = 0;
    for (i = 0; i < 20 * 2; i++)
    {
        c->mBrushSize = (i * 4 * 22) / (20 * 8);
        c->createBrush();

        mBackground->drawImage(pf->mCanvas->mBrush,7 + i * 4 - pf->mCanvas->mBrush->getWidth() / 2, 289 + 12 - pf->mCanvas->mBrush->getHeight() / 2);
    }
    c->mBrushSize = 22;

    for (i = 0; i < 20; i++)
    {
        c->mBrushHardness = (i * 32) / (20);
        c->createBrush();

        mBackground->drawImage(pf->mCanvas->mBrush,187 + i * 8 - pf->mCanvas->mBrush->getWidth() / 2, 289 + 12 - pf->mCanvas->mBrush->getHeight() / 2);
    }
    c->mBrushSize = 16;
    c->mBrushHardness = 0;
    c->createBrush();
}

void PaletteBackground::render()
{
    PaintForge *pf = PaintForge::getInstance();
    XFcGLSurface *fb = pf->mGL->getSecondary();

    XFcRectangle r = pf->mCanvas->mDirtyFrameRect;
    pf->mCanvas->mDirtyFrameRect = XFcRectangle(0,0,0,0);

    fb->drawImage(mBackground);

    fb->drawFilledRect(384,320-63,94,62,0xffffffff);
    fb->drawImage(pf->mCanvas->mSurf,480-97,320-63,94,62);
    if (pf->mCanvas->mUseWorkSurface)
        fb->drawImageBlend(pf->mCanvas->mWorkSurface,480-97,320-63,94,62,XFCBLEND_ALPHA,pf->mCanvas->mActiveAlpha);
//    PaintForge::getInstance()->mGL->getSecondary()->drawFilledRect(r.mX,r.mY,r.mWidth,r.mHeight,0xffffffff);    
}
