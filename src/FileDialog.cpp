//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//
#ifndef XFC_PLATFORM_DESKTOPWINDOWS
#define USE_TAPWAVE_NEW 0
#include <tapwave.h>
#endif
#include <xforge.h>
#include <xfutil/XFuPrinter.h>
#include "PaintForge.h"
#include "Widget.h"
#include "Canvas.h"
#include "FileDialog.h"

void FileDialog::init(INT32 aMode)
{
    PaintForge *pf = PaintForge::getInstance();
    pf->mTemporaryPalette = 0;
 
    mDialogMode = aMode;
    XFcFileFind * ff = XFcFileFind::create("*.pfi");
    mFileCount = 0;
    while (ff->next() != NULL) mFileCount++;
    delete ff;
    ff = XFcFileFind::create("*.pfi");
    
    mFiles = new CHAR*[mFileCount];
    mFileOffset = 0;
    mFileCount = 0;
    const CHAR * fn = ff->next();
    if (fn)
    {
        do
        {
            mFiles[mFileCount] = XFcStringToolkit::copy(fn);
            mFileCount++;
            fn = ff->next();
        }
        while (fn != NULL);
    }

    delete ff;
#ifdef XFC_FORCEDEVICE_TAPWAVE
    XFcExclusiveModeSettings settings;
    XFcCore::setExclusiveMode(0,settings); // set non-exclusive mode
    PINSetInputAreaState(pinInputAreaShow);     // show graffiti
    SysSetOrientationTriggerState(sysOrientationTriggerDisabled);
#endif
    mRefresh = 1;
}

void FileDialog::deinit()
{
    INT i;
    for (i = 0; i < mFileCount; i++)
        delete[] mFiles[i];
    delete[] mFiles;
    mFiles = NULL;
    mFileCount = mFileOffset = 0;
    memset(mCurrentFilename,0,19);
#ifdef XFC_FORCEDEVICE_TAPWAVE
    XFcExclusiveModeSettings settings;
    XFcCore::setExclusiveMode(1,settings); // set exclusive mode
    SysSetOrientationTriggerState(sysOrientationTriggerEnabled);
    PINSetInputAreaState(pinInputAreaHide);     // Hide graffiti
    StatHide();                                 // Hide status bar
#endif
    mRefresh = 1;
}

FileDialog::FileDialog()
{
    mHitRegion = XFcRectangle(0,0,480,320);
    mCurrentFilename = new CHAR[19];
    memset(mCurrentFilename,0,19);
    mFileCount = mFileOffset = 0;
    mFiles = NULL;
    mRefresh = 1;
}

void FileDialog::onCharEntered(INT32 aChar)
{
    if (mDialogMode == 1)
        return; 

    INT l = XFcStringToolkit::getLength(mCurrentFilename);
    if (aChar == 8) // backspace
    {
        if (l > 0)
            mCurrentFilename[l - 1] = 0;
        return;
    }
    if (l == 18) return;
    if (aChar > 32 && aChar < 'z')
    {
        mCurrentFilename[l] = (CHAR)aChar;
    }
    mRefresh = 1;
}

void FileDialog::render()
{
    if (!mRefresh)
        return;
    mRefresh = 0;
    PaintForge *pf = PaintForge::getInstance();
    XFcGLSurface *fb = pf->mGL->getSecondary();
    fb->drawImage(pf->mLogo);

    // title bar
    PaintForge::drawRect(fb,5,5,240,25,0xff000000);
    PaintForge::drawFilledRect(fb,6,6,239,24,0xffffffff,0x9fffffff);
    
    // file list
    PaintForge::drawFilledRect(fb,5,30,290,290,0xff000000,0x9fffffff);

    // scrollbar
    PaintForge::drawFilledRect(fb,295,30,315,290,0xff000000,0x9fffffff);
    
    // input field
    PaintForge::drawFilledRect(fb,5,295,240,315,0xff000000,0x9fffffff);
    pf->mPrinter->print(fb,9,297,mCurrentFilename,XFCBLEND_MUL_FAST);
    
    // "save" or "load"
    PaintForge::drawFilledRect(fb,245,295,315,315,0xff000000,0x9fffffff);

    // "cancel"
    PaintForge::drawFilledRect(fb,245,5,315,25,0xff000000,0x9fffffff);
    pf->mPrinter->print(fb,250,8,"Cancel",XFCBLEND_MUL_FAST);


    if (mDialogMode == 0)
    {
        pf->mPrinter->print(fb,9,7,"Save PaintForge Image..",XFCBLEND_MUL_FAST);
        pf->mPrinter->print(fb,259,297,"Save",XFCBLEND_MUL_FAST);
        INT32 w,h;
        pf->mPrinter->stringMetrics(mCurrentFilename,w,h);
        if ((XFcCore::getTick() >> 8) & 1)
            pf->mPrinter->print(fb,9 + w,297,"_",XFCBLEND_MUL_FAST);
    }
    else
    {
        pf->mPrinter->print(fb,9,7,"Load PaintForge Image..",XFCBLEND_MUL_FAST);
        pf->mPrinter->print(fb,259,297,"Load",XFCBLEND_MUL_FAST);
    }

    CHAR *tmp = XFcStringToolkit::allocatedFormat("%d",XFcCore::getFreePoolMemory());
    pf->mPrinter->print(fb,0,0,tmp,XFCBLEND_MUL_FAST);
    delete[] tmp;


    INT barheight;
    INT barstep;
    if (mFileCount > 16)
    {
        barheight = 254 * 16 / mFileCount;
        if (barheight > 254) barheight = 254;
        barstep = (254 - barheight) / (mFileCount - 16);
    }
    else
    {
        barheight = 254;
        barstep = 0;
    }


    fb->drawFilledRect(298,33 + mFileOffset * barstep,313-298,barheight,0x9f000000);

    INT i;
    for (i = 0; i < 16; i++)
    {
        if (mFileCount > (i + mFileOffset))
            pf->mPrinter->print(fb,9,32 + i * 16,mFiles[i + mFileOffset],XFCBLEND_MUL_FAST);
    }
}

INT FileDialog::load()
{
    PaintForge *pf = PaintForge::getInstance();
    if (mCurrentFilename[0] == 0)
        return 0;
    CHAR * fn = XFcStringToolkit::allocatedFormat("%s.pfi",mCurrentFilename);
    if (fn == NULL)
        return 0;
    XFcCFL *cf = XFcCFL::create(fn);
    if (cf == NULL)
        return 0;
    if (!cf->fileExists("Canv"))
    {
        delete cf;
        return 0;
    }

    deinit();

    pf->mCanvas->commitWork();
    INT32 *b;
    pf->mCanvas->mSurf->lock((void**)&b,XFCGF_DEFAULT,XFCGFX_DISCARDCONTENTS);
    cf->getFile("Canv",(INT8*)b,320*480*4);
    pf->mCanvas->mSurf->unlock();
    delete cf;
    return 1;
}

INT FileDialog::save()
{
    PaintForge *pf = PaintForge::getInstance();
    if (mCurrentFilename[0] == 0)
        return 0;
    CHAR * fn = XFcStringToolkit::allocatedFormat("%s.pfi",mCurrentFilename);
    if (fn == NULL)
        return 0;

    XFcCFLMaker * cf = XFcCFLMaker::create(fn);
    delete[] fn;
    if (cf == NULL)
        return 0;
    
    deinit();

    pf->mCanvas->commitWork();
    INT32 *b;
    pf->mCanvas->mSurf->lock((void**)&b,XFCGF_DEFAULT,XFCGFX_DISCARDCHANGES);
    cf->store("Canv",(const signed char *)b,320*480*4,0x00000001);
    pf->mCanvas->mSurf->unlock();
    cf->finish(0x00000001);
    return 1;
}


INT FileDialog::checkHit(INT32 aX, INT32 aY, INT32 aEvent)
{
    mRefresh = 1;
    PaintForge *pf = PaintForge::getInstance();
    if (aEvent == 1)
    {
        mActiveWidget = 0;
        if (Widget::checkHit(aX,aY,0,&XFcRectangle(295,30,315-295,290-30)))
        {
            mActiveWidget = 1;
        }    
    }

    if (mActiveWidget == 1)
    {
        if (aEvent == -1)
            mActiveWidget = 0;
        if (mFileCount < 16)
            return 0;
        INT y = aY - 30;
        if (y > 254) y = 254;
        if (y < 0) y = 0;
        INT barheight = 254 * 16 / mFileCount;
        if (barheight > 254) barheight = 254;

        INT barstep = (254 - barheight) / (mFileCount - 16);

        if (y < barheight / 2) y = barheight / 2;
        if (y > 254 - (barheight / 2)) y = 254 - (barheight / 2);
        y -= barheight / 2;
        y /= barstep;
        mFileOffset = y;
        return 0;
    }    

    if (aEvent != -1) 
        return 0;  

    if (Widget::checkHit(aX,aY,0,&XFcRectangle(245,5,315-245,25-5)))
    {
        pf->mActiveWidgets = pf->mPaletteWidgets;
        pf->mCanvas->addToDirtyRect(0,0,480,320,pf->mCanvas->mDirtyFrameRect);
        deinit();
        return 0;
    }
    if (Widget::checkHit(aX,aY,0,&XFcRectangle(245,295,315-245,315-295)))
    {
        if (mDialogMode == 0)
        {
            if (!save())
                return 0;
        }
        else
        {
            if (!load())
                return 0;
        }
        pf->mActiveWidgets = pf->mCanvas;
        pf->mCanvas->addToDirtyRect(0,0,480,320,pf->mCanvas->mDirtyFrameRect);
        return 0;
    }
    if (Widget::checkHit(aX,aY,0,&XFcRectangle(5,30,290-5,290-30)))
    {
        INT y = aY - 30;
        y /= 16;
        if (y + mFileOffset > mFileCount)
            return 0;
        memset(mCurrentFilename,0,19);
        INT i = 0;
        while (i < 18 && mFiles[y + mFileOffset][i] != 0 && mFiles[y + mFileOffset][i] != '.')
        {
            mCurrentFilename[i] = mFiles[y + mFileOffset][i];
            i++;
        }
        return 0;
    }


    return 0;
}