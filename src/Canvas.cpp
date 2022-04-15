//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#include <xforge.h>
#include <xfutil/XFuPrinter.h>
#include "PaintForge.h"
#include "Canvas.h"

INT32 Canvas::biasedAlpha()
{
    INT32 v = mAlpha + mAlphaBias;
    if (v < 0) v = 0;
    if (v > 0xff) v = 0xff;
    return v;
}

INT32 Canvas::biasedSize()
{
    INT32 v = mBrushSize + mSizeBias;
    if (v < 2) v = 2;
    if (v > 48) v = 48;
    return v;    
}


Canvas::Canvas()
{    
    mWorkSurface = NULL;
    mSurf = NULL;
    mBrush = NULL;
    mWorkSurface = XFcGLSurface::create(480, 320, XFCGF_PREMULA8R8G8B8);//XFCGF_A8R8G8B8);
    mSurf = XFcGLSurface::create(480, 320, XFCGF_PREMULA8R8G8B8);//XFCGF_A8R8G8B8);
    mWorkSurface->clear(0);
    mSurf->clear(0);
    mBrush = NULL;
    mBrushSize = 16;
    mBrushHardness = 0;
    mColor = 0;
    mColorHSV = rgb2hsv(mColor);
    mAlpha = 0x6;
    mBrushMode = 1;
    mSizeBias = 0;
    mAlphaBias = 0;

    mBrushPrecalc = new UINT8[256*256];
    INT32 halfsize = 256 / 2;
    INT32 multiplier = (256 / halfsize);
    INT32 ofs = 0;
    for (INT32 y = 0; y < 256; y++)
    {
        for (INT32 x = 0; x < 256; x++, ofs++)
        {
            INT xv = halfsize - x;
            INT yv = halfsize - y;
            INT v = (INT)sqrt((float)xv*(float)xv + (float)yv*(float)yv);
            INT alpha = 0xff - (int)(v * multiplier);
            if (alpha < 0)
                alpha = 0;
            else
            if (alpha > 0xff)
                alpha = 0xff;
            mBrushPrecalc[ofs] = (UINT8)alpha;
        }
    }
    createBrush();

}


void Canvas::changeBias(INT32 aSizeBias, INT32 aAlphaBias)
{
    mSizeBias = aSizeBias / (32768 / 24);
    mAlphaBias = aAlphaBias / (32768 / 128);
    createBrush();
}


void Canvas::commitWork()
{
    if (mUseWorkSurface)
    {
        mSurf->drawImageBlend(mWorkSurface, 
                                  mDirtyWorkRect.mX, 
                                  mDirtyWorkRect.mY, 
                                  mDirtyWorkRect, 
                                  mDirtyWorkRect.mWidth, 
                                  mDirtyWorkRect.mHeight, 
                                  XFCBLEND_ALPHA, 
                                  mActiveAlpha);
    }
    clearWorkSurface();
}


void Canvas::effectUndo()
{
    mUseWorkSurface = 1 - mUseWorkSurface;
    mDirtyFrameRect = mDirtyWorkRect;
}


void Canvas::effectClear()
{
    commitWork();
    mUseWorkSurface = 1;
    mActiveAlpha = 0xff;
    mWorkSurface->clear(mColor | (0xff000000));
    addToDirtyRect(0,0,480,320,mDirtyFrameRect);
    mDirtyWorkRect = mDirtyFrameRect;
}


void Canvas::clearWorkSurface()
{
    //mWorkSurface->clear(0);
    mDirtyFrameRect = mDirtyWorkRect;
    UINT32 *px;
    mWorkSurface->lock((void**)&px,XFCGF_A8R8G8B8,0);
    INT32 i,j;
    for (i = 0; i < mDirtyWorkRect.mHeight; i++)
    {
        INT32 ofs = ((mDirtyWorkRect.mY + i) * mWorkSurface->getWidth()) + mDirtyWorkRect.mX;
        for (j = 0; j < mDirtyWorkRect.mWidth; j++)
        {
            px[ofs + j] = 0x0;
        }
    }
    mWorkSurface->unlock();
    mDirtyWorkRect.mHeight = 0;
    mDirtyWorkRect.mWidth = 0;
    mDirtyWorkRect.mX = 0;
    mDirtyWorkRect.mY = 0;
    
}


// Enlarges the dirty rectangle
void Canvas::addToDirtyRect(INT32 aX, INT32 aY, INT32 aWidth, INT32 aHeight, XFcRectangle &aRect)
{
    if (aRect.mHeight == 0 || aRect.mWidth == 0)
    {
        aRect.mX = aX;
        aRect.mY = aY;
        aRect.mWidth = aWidth;
        aRect.mHeight = aHeight;
    }
    else
    {
        if (aX < aRect.mX) 
        {
            aRect.mWidth += aRect.mX - aX;
            aRect.mX = aX;
        }
        if (aY < aRect.mY)
        {
            aRect.mHeight += aRect.mY - aY;
            aRect.mY = aY;
        }
        if (aWidth + aX > aRect.mWidth + aRect.mX)
            aRect.mWidth = (aWidth + aX) - (aRect.mX);
        if (aHeight + aY > aRect.mHeight + aRect.mY)
            aRect.mHeight = (aHeight + aY) - (aRect.mY);
    }
    if (aRect.mX < 0) aRect.mX = 0;
    if (aRect.mY < 0) aRect.mY = 0;
    if (aRect.mWidth + aRect.mX > 480) aRect.mWidth = 480 - aRect.mX;
    if (aRect.mHeight + aRect.mY > 320) aRect.mHeight = 320 - aRect.mY;
}

static INT32 smoothstep(REAL a, REAL b, REAL x)
{
    if (x < a) return 0;
    if (x >= b) return 0xff;
    x = (x - a) / (b - a);
    return (INT32)((x*x*(3-2*x))*0xff);
}

void Canvas::createBrush()
{
    INT32 ramp[256];
    INT32 i;
    for (i = 0; i < 0xff; i++)
        ramp[i] = smoothstep(0 + mBrushHardness * 4,0xff - mBrushHardness * 4,i);

    INT32 size = biasedSize();
    delete mBrush;
    mBrush = XFcGLSurface::create(size, size, XFCGF_PREMULA8R8G8B8);
    
    // Lock the surface:
    INT32 *map;
    INT32 pitch = mBrush->lock((void **)&map,XFCGF_A8R8G8B8);  
    // Pitch is divided by 2 because we're using a 16-bit pointer
    // and pitch is in bytes.
    pitch /= 4;
    
    // Fill the surface:
    INT32 ofs = 0;
    INT32 color = mColor;
/*
    INT32 halfsize = size / 2;
    INT32 multiplier = (256 / halfsize);
    for (INT32 y = 0; y < mBrush->getHeight(); y++, ofs = y * pitch)
    {
        for (INT32 x = 0; x < mBrush->getWidth(); x++, ofs++)
        {
            INT xv = halfsize - x;
            INT yv = halfsize - y;
            INT v = XFcMath::sqrt(xv*xv + yv*yv);
            INT alpha = 0xff - (int)(v * multiplier);
            if (alpha < 0)
                alpha = ramp[0];
            else
            if (alpha > 0xff)
                alpha = ramp[0xff];
            else
                alpha = ramp[alpha];
            map[ofs] = (color & 0xffffff) | (alpha << 24);
        }
    }
*/    
    REAL yi = REALi(0xff) / mBrush->getHeight();
    REAL xi = REALi(0xff) / mBrush->getWidth();
    REAL yy = 0;//yi / 2;

    for (INT32 y = 0; y < mBrush->getHeight(); y++, ofs = y * pitch)
    {
        REAL xx = 0;//xi / 2;
        yy += yi;
        for (INT32 x = 0; x < mBrush->getWidth(); x++, ofs++)
        {
            INT32 alpha = ramp[mBrushPrecalc[((INT32)xx) + (((INT32)yy) << 8)]];
            map[ofs] = (color & 0xffffff) | (alpha << 24);
            xx += xi;            
        }
    }
    // and remember to unlock:
    mBrush->unlock();
}


void Canvas::render()
{
    XFcGLSurface *fb = PaintForge::getInstance()->mGL->getSecondary();
    PaintForge *pf = PaintForge::getInstance();

    // TODO; clip everything to dirtyframerect!    
    /*
    INT32 i, j;
    for (i = 0; i < 480 / BACKTILE_SIZE; i++)
    {
        for (j = 0; j < 320 / BACKTILE_SIZE; j++)
        {
            fb->drawFilledRect(i * BACKTILE_SIZE, 
                               j * BACKTILE_SIZE, 
                               BACKTILE_SIZE, 
                               BACKTILE_SIZE, 
                               0xff000000 | (((i&1)^(j&1))?CHECKER_DARK:CHECKER_LIGHT));
        }
    }*/
    fb->drawFilledRect(mDirtyFrameRect.mX, 
                  mDirtyFrameRect.mY, 
                  mDirtyFrameRect.mWidth, 
                  mDirtyFrameRect.mHeight,0xffffffff);

    //fb->drawImage(mSurf);
    fb->drawImage(mSurf, 
                  mDirtyFrameRect.mX, 
                  mDirtyFrameRect.mY, 
                  mDirtyFrameRect, 
                  mDirtyFrameRect.mWidth, 
                  mDirtyFrameRect.mHeight);

    if (mUseWorkSurface)
    {
        fb->drawImageBlend(mWorkSurface, 
                              mDirtyFrameRect.mX, 
                              mDirtyFrameRect.mY, 
                              mDirtyFrameRect, 
                              mDirtyFrameRect.mWidth, 
                              mDirtyFrameRect.mHeight, 
                              XFCBLEND_ALPHA, mActiveAlpha);
    }
    mDirtyFrameRect.mWidth = mDirtyFrameRect.mHeight = 0;
}

INT Canvas::checkHit(INT32 aX, INT32 aY, INT32 aEvent)
{
    PaintForge *pf = PaintForge::getInstance();
    if (aEvent == 1)
    {
        commitWork();
        mLastX = aX;
        mLastY = aY;
        if (mBrushMode)
            mActiveAlpha = 0xff;
        else
            mActiveAlpha = mAlpha;
        mUseWorkSurface = 1;
    }
    if (aEvent == -1)
    {
        return 1;
    }

    INT32 x = mLastX;
    INT32 y = mLastY;
    REAL rx = mLastX;
    REAL ry = mLastY;

    INT32 bs = biasedSize();

    addToDirtyRect(mLastX - (bs / 2), mLastY - (bs / 2), bs, bs,mDirtyFrameRect);
    addToDirtyRect(aX - (bs / 2), aY - (bs / 2), bs, bs,mDirtyFrameRect);
    addToDirtyRect(mLastX - (bs / 2), mLastY - (bs / 2), bs, bs,mDirtyWorkRect);
    addToDirtyRect(aX - (bs / 2), aY - (bs / 2), bs, bs,mDirtyWorkRect);

    REAL rxd = (aX - mLastX);
    REAL ryd = (aY - mLastY);    

    REAL length = XFcMath::abs(rxd) > XFcMath::abs(ryd) ? XFcMath::abs(rxd) : XFcMath::abs(ryd);
    INT32 step = bs / 8;
    if (step < 1) step = 1;
    length /= step;
    //REAL length = sqrt((float)rxd * (float)rxd + (float)ryd * (float)ryd);
    if (length != 0)
    {
        ryd /= length;
        rxd /= length;
    }
    else
    {
        ryd = rxd = 0;
    }

    if (rxd == 0) rx = aX;
    if (ryd == 0) ry = aY;

    do
    {
        if (x != aX) x = rx;
        if (y != aY) y = ry;
        if (mBrushMode)
            mWorkSurface->drawImageBlend(mBrush,x - mBrush->getWidth() / 2, y  - mBrush->getHeight() / 2, XFCBLEND_ALPHA, biasedAlpha());
        else
            mWorkSurface->drawImage(mBrush,x - mBrush->getWidth() / 2, y  - mBrush->getHeight() / 2);
        rx += rxd;
        ry += ryd;
        if (XFcMath::abs(rx - aX) < step) rx = aX;
        if (XFcMath::abs(ry - aY) < step) ry = aY;
    }
    while (x != aX || y != aY);
    mLastX = aX;
    mLastY = aY;

    return 1;
}

