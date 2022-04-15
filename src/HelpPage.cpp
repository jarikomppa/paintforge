//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//
#include <xforge.h>
#include <xfutil/XFuPrinter.h>
#include "PaintForge.h"
#include "Widget.h"
#include "Canvas.h"
#include "HelpPage.h"

static const CHAR lorem[] = 
"Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n\n"
"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat.\n\n"
"Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat. Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis at vero eros et accumsan et iusto odio dignissim qui blandit praesent luptatum zzril delenit augue duis dolore te feugait nulla facilisi.\n\n"
"Nam liber tempor cum soluta nobis eleifend option congue nihil imperdiet doming id quod mazim placerat facer possim assum. Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat.\n\n"
"Duis autem vel eum iriure dolor in hendrerit in vulputate velit esse molestie consequat, vel illum dolore eu feugiat nulla facilisis. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, At accusam aliquyam diam diam dolore dolores duo eirmod eos erat, et nonumy sed tempor et et invidunt justo labore Stet clita ea et gubergren, kasd magna no rebum. sanctus sea sed takimata ut vero voluptua. est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat.\n\n"
"Consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.\n\n";

static const CHAR about[] =
"\n"
"\n"
"PaintForge 0.5b\n"
"Copyright 2003-2004 Jari Komppa\n"
"\n"
"Homepage: http://iki.fi/sol/\n"
"\n"
"\n"
"\n"
"This is a (more or less) proof of concept version of PaintForge.\n"
"\n"
"It is by no means a final product, even if it is possible to draw something with it. =)\n"
"\n"
"\n"
"Since you got this far, you've noticed that left shoulder button opens the tools.\n"
"\n"
"\n"
"Other commands are as follows:\n"
"\n"
"Tap left shoulder button - enter or exit tools menu.\n"
"\n"
"If you depress left shoulder button, use the tools, and release the shoulder button, you will "
"return to the canvas.\nThis way you can quickly hop between tools and the canvas.\n"
"\n"
"Tap right shoulder button - undo/redo.\n"
"\n"
"Joystick left/right - brush size bias.\n"
"\n"
"Joystick up/down - brush alpha bias (only affects airbrush).\n"
"\n"
"\n"
"That's all folks.. Send me your pictures! =)\n";


void HelpPage::init(INT32 aMode)
{
    PaintForge *pf = PaintForge::getInstance();
    pf->mTemporaryPalette = 0;

    mDialogMode = aMode;
    if (aMode == 1)
        mText = (CHAR*)lorem;
    if (aMode == 0)
        mText = (CHAR*)about;

    mLineCount = pf->mPrinter->print(pf->mGL->getSecondary(),&XFcRectangle(0,0,440,256),mText) / 16;
    
    mRefresh = 1;
}

void HelpPage::deinit()
{
}

HelpPage::HelpPage()
{
    mHitRegion = XFcRectangle(0,0,480,320);
    mLineCount = mLineOffset = 0;
    mText = NULL;
    mRefresh = 1;
}

void HelpPage::render()
{
    if (!mRefresh)
        return;
    mRefresh = 0;
    PaintForge *pf = PaintForge::getInstance();
    XFcGLSurface *fb = pf->mGL->getSecondary();
    fb->drawImage(pf->mLogo);
    
    // title bar
    PaintForge::drawRect(fb,5,5,475,25,0xff000000);
    PaintForge::drawFilledRect(fb,6,6,474,24,0xffffffff,0x9fffffff);

    // text area
    PaintForge::drawFilledRect(fb,5,30,450,290,0xff000000,0x9fffffff);

    // scrollbar
    PaintForge::drawFilledRect(fb,455,30,475,290,0xff000000,0x9fffffff);
       
    // "ok"
    PaintForge::drawFilledRect(fb,160,295,360,315,0xff000000,0x9fffffff);
    pf->mPrinter->print(fb,246,298,"Ok",XFCBLEND_MUL_FAST);


    INT barheight;
    INT barstep;
    if (mLineCount > 16)
    {
        barheight = 254 * 16 / mLineCount;
        if (barheight > 254) barheight = 254;
        barstep = (254 - barheight) / (mLineCount - 16);
    }
    else
    {
        barheight = 254;
        barstep = 0;
    }

    fb->drawFilledRect(458,33 + mLineOffset * barstep,313-298,barheight,0x9f000000);

    switch (mDialogMode)
    {
    case 0:
        pf->mPrinter->print(fb,9,7,"About PaintForge",XFCBLEND_MUL_FAST);
        break;
    case 1:
        pf->mPrinter->print(fb,9,7,"Lorem ipsum dolor set..",XFCBLEND_MUL_FAST);
        break;
    }

    XFcGLSurface * s = XFcGLSurface::create(440,256);
    s->clear(0xffffffff);
    pf->mPrinter->print(s, &XFcRectangle(0,-16*mLineOffset,440,256), mText, XFCBLEND_MUL_FAST);
    fb->drawImageBlend(s,10,35,XFCBLEND_MUL_FAST,0);
    delete s;
}


INT HelpPage::checkHit(INT32 aX, INT32 aY, INT32 aEvent)
{
    mRefresh = 1;
    PaintForge *pf = PaintForge::getInstance();
    if (aEvent == 1)
    {
        mActiveWidget = 0;
        if (Widget::checkHit(aX,aY,0,&XFcRectangle(455,30,315-295,290-30)))
        {
            mActiveWidget = 1;
        }    
    }

    if (mActiveWidget == 1)
    {
        if (aEvent == -1)
            mActiveWidget = 0;
        if (mLineCount < 16)
            return 0;
        INT y = aY - 30;
        if (y > 254) y = 254;
        if (y < 0) y = 0;
        INT barheight = 254 * 16 / mLineCount;
        if (barheight > 254) barheight = 254;

        INT barstep = (254 - barheight) / (mLineCount - 16);

        if (y < barheight / 2) y = barheight / 2;
        if (y > 254 - (barheight / 2)) y = 254 - (barheight / 2);
        y -= barheight / 2;
        y /= barstep;
        mLineOffset = y;
        return 0;
    }    

    if (aEvent != -1) 
        return 0;  

    if (Widget::checkHit(aX,aY,0,&XFcRectangle(160,295,360-160,315-295)))
    {
        pf->mActiveWidgets = pf->mPaletteWidgets;
        pf->mCanvas->addToDirtyRect(0,0,480,320,pf->mCanvas->mDirtyFrameRect);
        deinit();
        return 0;
    }

    return 0;
}