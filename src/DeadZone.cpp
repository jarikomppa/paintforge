//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#include <xforge.h>
#include "PaintForge.h"
#include "DeadZone.h"

DeadZone::DeadZone(XFcRectangle aRect, UINT32 aClearColor)
{
    mHitRegion = aRect;
    mClearColor = aClearColor;
}

void DeadZone::render()
{
    if (mClearColor)
        PaintForge::getInstance()->mGL->getSecondary()->drawFilledRect(mHitRegion.mX,mHitRegion.mY,mHitRegion.mWidth,mHitRegion.mHeight,mClearColor);
}

