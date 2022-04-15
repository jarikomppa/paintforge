//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef CANVAS_H_INCLUDED
#define CANVAS_H_INCLUDED

#include "Widget.h"

class Canvas : public Widget
{
public:
    Canvas();
    virtual void render();
    virtual INT checkHit(INT32 aX, INT32 aY, INT32 aEvent);
    void addToDirtyRect(INT32 aX, INT32 aY, INT32 aWidth, INT32 aHeight, XFcRectangle &aRect);    
    void createBrush();
    void clearWorkSurface();
    void commitWork();
    void changeBias(INT32 aSizeBias, INT32 aAlphaBias);
    INT32 biasedAlpha();
    INT32 biasedSize();


    void effectUndo();
    void effectClear();
    void effectBrushmode() { mBrushMode = 1 - mBrushMode; }

    UINT8 *mBrushPrecalc;
    XFcRectangle mDirtyWorkRect;
    XFcRectangle mDirtyFrameRect;
    XFcGLSurface *mWorkSurface;
    XFcGLSurface *mSurf;
    XFcGLSurface *mBrush;
    Widget *mNext;
    XFcRectangle mHitRegion;
    INT mBrushMode;
    INT32 mLastX, mLastY;
    INT32 mActiveAlpha;
    INT32 mBrushSize;
    INT32 mBrushHardness;
    INT32 mColor;
    INT32 mColorHSV;
    INT32 mAlpha;
    INT32 mSizeBias;
    INT32 mAlphaBias;
    INT mUseWorkSurface;
};

#endif // PFWIDGET_H_INCLUDED