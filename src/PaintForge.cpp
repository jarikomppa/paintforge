//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#include <xforge.h>
#include <xfutil/XFuPrinter.h>
#include <xfutil/XFuFPSCount.h>
#include "PaintForge.h"
#include "Widget.h"
#include "DeadZone.h"
#include "Canvas.h"
#include "BrushSize.h"
#include "BrushPreview.h"
#include "RGBAPicker.h"
#include "Swatches.h"
#include "Dropdown.h"
#include "PaletteBackground.h"
#include "FileDialog.h"
#include "HelpPage.h"

#define SHOW_FPS    

INT32 rgb2hsv(INT32 aRGB)
{
    float r = ((aRGB >> 16) & 0xff) / 255.0f;
    float g = ((aRGB >>  8) & 0xff) / 255.0f;
    float b = ((aRGB >>  0) & 0xff) / 255.0f;
    float min = r;
    if (min > g) min = g;
    if (min > b) min = b;
    float max = r;
    if (max < g) max = g;
    if (max < b) max = b;
    float v = max; // this is the value v
    float s;
    float h;
    if (max != 0)
    {
        s = (max - min) / max; // s is the saturation
    }
    else
    {
        s = 0; // black has no saturation
    }

    if (s == 0)
    {
        h = 0; // well, hue should be undefined in this case
    }
    else
    {
        float delta = max - min;
        if (r == max) 
        {
            h = (g - b) / delta;
        }
        else
        if (g == max)
        {
            h = 2 + (b - r) / delta;
        }
        else
        if (b == max)
        {
            h = 4 + (r - g) / delta;
        }
        h = h * 60; 
        if (h < 0)
            h += 360;
    }
    h /= 360;
    h *= 255;
    s *= 255;
    v *= 255;
    INT32 result = (((INT)floor(h))<<16) | (((INT)floor(s))<<8) | (((INT)floor(v))<<0);
    return result;
}

INT32 hsv2rgb(INT32 aHSV)
{
    float h = (((aHSV >> 16) & 0xff) / 256.0f) * 360;
    float s = ((aHSV >>  8) & 0xff) / 256.0f;
    float v = ((aHSV >>  0) & 0xff) / 256.0f;

    float r,g,b;

    if (s == 0)
    {
        r = g = b = v;
    }
    else
    {
        if (h >= 360) h -= 360;
        h /= 60;
        int i = (int)floor(h);
        float f = h - i;
        float p = v * (1 - s);
        float q = v * (1 - (s * f));
        float t = v * (1 - (s * (1 - f)));
        switch (i)
        {
        case 0:
            r = v;
            g = t;
            b = p;
            break;
        case 1:
            r = q;
            g = v;
            b = p;
            break;
        case 2:
            r = p;
            g = v;
            b = t;
            break;
        case 3:
            r = p;
            g = q;
            b = v;
            break;
        case 4:
            r = t;
            g = p;
            b = v;
            break;
        case 5:
            r = v;
            g = p;
            b = q;
            break;
        }
    }
    r *= 255;
    g *= 255;
    b *= 255;
    INT32 result = (((INT)floor(r))<<16) | (((INT)floor(g))<<8) | (((INT)floor(b))<<0);
    return result;
}


PaintForge::PaintForge()
{
    mGL = NULL;
    mActiveWidgets = NULL;
    mPaletteWidgets = NULL;
    mCaptured = NULL;
    mRunLevel = 0;
    mDoneSplash = 0;
    INT32 i;
    for (i = 0; i < 32; i++)
        mKeyDownBuffer[i] = 0;
}

void PaintForge::onJoystickMoved(INT32 aX, INT32 aY)
{
    mCanvas->changeBias(aX, aY);
}

void PaintForge::onCharEntered(CHAR aChar)
{
    if (mActiveWidgets == mFileDialog)
        mFileDialog->onCharEntered(aChar);
}

void PaintForge::drawRect(XFcGLSurface *aSurf, INT32 aX1, INT32 aY1, INT32 aX2, INT32 aY2, INT32 aColor)
{
    aSurf->drawFilledRect(aX1,aY1,aX2-aX1+1,1,aColor);
    aSurf->drawFilledRect(aX1,aY2,aX2-aX1+1,1,aColor);
    aSurf->drawFilledRect(aX1,aY1,1,aY2-aY1+1,aColor);
    aSurf->drawFilledRect(aX2,aY1,1,aY2-aY1+1,aColor);
}

void PaintForge::drawFilledRect(XFcGLSurface *aSurf, INT32 aX1, INT32 aY1, INT32 aX2, INT32 aY2, INT32 aColor, INT32 aFillColor)
{
    aSurf->drawFilledRect(aX1+1,aY1+1,(aX2-aX1)-1,(aY2-aY1)-1,aFillColor);
    drawRect(aSurf,aX1,aY1,aX2,aY2,aColor);
}


void PaintForge::onControlDown(INT32 aCode)
{
    INT32 *p = mKeyDownBuffer;
    while (*p != 0 && *p != aCode) p++;
    if (*p == 0)
    {
        *p = aCode;
    }
    else
        return; // duplicate keypress

    if (mRunLevel <= SPLASH_IDLING) return;
    if (aCode == XFCCL_FIRE1)
    {
        if (mActiveWidgets == mCanvas)
        {
            mActiveWidgets = mPaletteWidgets;
            mTemporaryPalette = 0;
        }
        else
        if (mActiveWidgets == mPaletteWidgets)
            mActiveWidgets = mCanvas;

        mCanvas->addToDirtyRect(0,0,480,320,mCanvas->mDirtyFrameRect);
    }
}


void PaintForge::onControlUp(INT32 aCode)
{
    INT32 *p = mKeyDownBuffer;
    while (*p != 0 && *p != aCode) p++;
    if (*p == 0)
        return; // unexpected key release (from launch?)
    do
    {
        *p = *(p + 1);
        p++;
    }
    while (*p != 0);

    if (mRunLevel < SPLASH_IDLING) return;
    if (mRunLevel == SPLASH_IDLING) 
    {
        mActiveWidgets = mCanvas;
        mRunLevel++;
        mCanvas->addToDirtyRect(0,0,480,320,mCanvas->mDirtyFrameRect);
        mDoneSplash = 1;
        return;
    }
    if (aCode == XFCCL_FIRE1)
    {
        if (mTemporaryPalette)
        {
            mActiveWidgets = mCanvas;
            mCanvas->addToDirtyRect(0,0,480,320,mCanvas->mDirtyFrameRect);
        }
    }
    if (aCode == XFCCL_FIRE2 && mCaptured == NULL) // undo / redo (not during draw op)
        mCanvas->effectUndo();
}



void PaintForge::onPointerUp(INT32 aX, INT32 aY)
{
    if (mRunLevel < SPLASH_IDLING) return;
    if (mRunLevel == SPLASH_IDLING) 
    {
        mActiveWidgets = mCanvas;
        mRunLevel++;
        mCanvas->addToDirtyRect(0,0,480,320,mCanvas->mDirtyFrameRect);
        mDoneSplash = 1;
        return;
    }
    if (mCaptured)
    {
        mCaptured->checkHit(aX, aY, -1);
        mCaptured = NULL;
        return;
    }
    Widget *walker = mActiveWidgets;
    while (walker)
    {
        if (walker->checkHit(aX, aY, -1))
            return;
        walker = walker->mNext;
    }
}




void PaintForge::onPointerMove(INT32 aX, INT32 aY)
{
    if (mRunLevel <= SPLASH_IDLING) return;
    if (mCaptured)
    {
        mCaptured->checkHit(aX, aY, 0);
        return;
    }
    Widget *walker = mActiveWidgets;
    while (walker)
    {
        if (walker->checkHit(aX, aY, 0))
            return;
        walker = walker->mNext;
    }

}


void PaintForge::onPointerDown(INT32 aX, INT32 aY)
{
    if (mRunLevel <= SPLASH_IDLING) return;
    Widget *walker = mActiveWidgets;
    
    if (mActiveWidgets == mPaletteWidgets)
    {
        mTemporaryPalette = 1;
    }
    
    while (walker)
    {
        if (walker->checkHit(aX, aY, 1))
        {
            mCaptured = walker;
            return;
        }
        walker = walker->mNext;
    }
}


void PaintForge::render()
{
    mFPS->tick();
    if (mRunLevel == 0)
    {        
        mGL->getSecondary()->drawImage(mLogo);
        mRunLevel++;
        mDoneSplash = 1;
    }
    if (mRunLevel == 1)
    {
        // TODO: perform most of initialization here
        mRunLevel++;
    }
    if (mRunLevel == SPLASH_IDLING)
    {
        if ((XFcCore::getTick() < 6000) || mDoneSplash)
            return;
        mRunLevel++;
        mCanvas->addToDirtyRect(0,0,480,320,mCanvas->mDirtyFrameRect);
    }
    Widget *list[64]; // max 64 widgets
    INT32 maxWidget = 0;

    Widget * walker = mActiveWidgets;
    while (walker)
    {
        list[maxWidget] = walker;
        maxWidget++;
        walker = walker->mNext;
    }
    
    INT32 i;
    for (i = maxWidget - 1; i >= 0; i--)
        list[i]->render();

#ifdef SHOW_FPS    
    CHAR temp[60];
    XFcStringToolkit::format(temp,"%3.3f",(float)mFPS->getFPS());
    drawFilledRect(mGL->getSecondary(),0,320-22,9*8,319,0xff000000,0xafffefef);
    mPrinter->print(mGL->getSecondary(),2,302,temp);
#endif
}


void PaintForge::onAppInit()
{
    mGL = XFcGL::create();
    mLogo = XFcImageLoader::load("logo.pcx");


    mPrinter = XFuPrinter::create("font16.pcx",0xffffffff);    
    mActiveWidgets = mCanvas = new Canvas();
    mPaletteWidgets = new PaletteBackground;
    ((PaletteBackground*)mPaletteWidgets)->init();
    mFileDialog = new FileDialog();
    mHelpPage = new HelpPage();

    Widget *w;
    w = new BrushSize();
    w->mNext = mPaletteWidgets; mPaletteWidgets = w;
    w = new BrushPreview();
    w->mNext = mPaletteWidgets; mPaletteWidgets = w;
    w = new RGBAPicker();
    w->mNext = mPaletteWidgets; mPaletteWidgets = w;
    w = new Swatches();
    w->mNext = mPaletteWidgets; mPaletteWidgets = w;
    w = new Dropdown();
    w->mNext = mPaletteWidgets; mPaletteWidgets = w;

    mFPS = XFuFPSCount::create(200);
    

    INT32 i,j;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 10; j++)
        {
            if (i == 4)
            {
                mSwatches[i * 10 + j] = hsv2rgb(0x000000|(((j*255) / 9)<<0));
            }
            else
            {
                INT32 h = ((i * 51)<<16);
                INT32 s = ((10 - j) * 255) / 5;
                if (s > 0xff) s = 0xff;
                s <<= 8;
                INT32 v = (((j + 1)*255) / 5);
                if (v > 0xff) v = 0xff;
                mSwatches[i * 10 + j] = hsv2rgb(s|h|v);
            }
        }
    }

}


void xfcAppPref(XFcAppPrefs &aAppPrefs)
{
#ifdef _DEBUG    
    aAppPrefs.mTitleString = XFCSTR("PaintForge - Debug - X-Forge/Windows");
#else
    aAppPrefs.mTitleString = XFCSTR("PaintForge - Release - X-Forge/Windows");
#endif
    aAppPrefs.mTotalMemoryLimit = (INT32)(1024 * 1024 * 6);
    aAppPrefs.mResourceMemoryLimit = (INT32)(1024 * 1024 * 6);
    aAppPrefs.mMinimumExtraMemory = (INT32)(1024 * 128);
    aAppPrefs.mPreferredWidth = 480;
    aAppPrefs.mPreferredHeight = 320;
}


INT32 xfcAppInit() 
{
    xfcUseDefaults();
    PaintForge *a = new PaintForge();
    XFcCore::setRenderer(a);
    XFcCore::setController(a);
    return 0;
}
