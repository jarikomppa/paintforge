//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#include <xforge.h>
#include <xfutil/XFuPrinter.h>
#include "PaintForge.h"
#include "Widget.h"
#include "Canvas.h"
#include "Dropdown.h"
#include "FileDialog.h"
#include "HelpPage.h"

struct MenuItem
{
    char * mTitle;
    INT32 mFunction;
};

#define MENU1_ITEMS 4
#define MENU2_ITEMS 3
#define MENU3_ITEMS 1
#define MENU4_ITEMS 4

enum MENUFUNCTIONS
{
    MENU_NONE = 0,
    MENU_QUIT,
    MENU_UNDO,
    MENU_CLEAR,
    MENU_SPLASH,
    MENU_ABOUTPF,
    MENU_LOREMIPSUM,
    MENU_BRUSHMODE,
    MENU_SAVE,
    MENU_LOAD
};

static const INT32 MenuItems[4] = {MENU1_ITEMS,MENU2_ITEMS,MENU3_ITEMS,MENU4_ITEMS};

static const MenuItem menu[4][8] =
{
    {
        "Load..",MENU_LOAD,
        "Save..",MENU_SAVE,
        "",MENU_NONE,
        "Quit",MENU_QUIT
    },
    {
        "Undo/Redo",MENU_UNDO,
        "Brush mode",MENU_BRUSHMODE,
        "Clear",MENU_CLEAR
    },
    {
        "Flatten",MENU_NONE
    },
    {
        "About PF..",MENU_ABOUTPF,
        "",MENU_NONE,
        "LoremIpsum",MENU_LOREMIPSUM,
        "Show splash",MENU_SPLASH
    }
};


Dropdown::Dropdown()
{
    mHitRegion = XFcRectangle(2,0,318,18);
    mActiveMenu = 0;
    mActiveMenuItem = 0;
}


void Dropdown::render()
{
    PaintForge *pf = PaintForge::getInstance();
    XFcGLSurface *fb = pf->mGL->getSecondary();

  
    pf->drawFilledRect(fb,2,0,320,20,0xff000000,0xafffffff);
    
    if (mActiveMenu)
        fb->drawFilledRect(2+(mActiveMenu - 1) * 80,2,80,18,0xffffffff);
    
    pf->mPrinter->print(fb,8,3,"File",XFCBLEND_MUL_FAST);
    pf->mPrinter->print(fb,8+80,3,"Effects",XFCBLEND_MUL_FAST);
    pf->mPrinter->print(fb,8+80+80,3,"Layers",XFCBLEND_MUL_FAST);
    pf->mPrinter->print(fb,8+80+80+80,3,"Help",XFCBLEND_MUL_FAST);
   
    fb->drawFilledRect(2+80,0,1,20,0xff000000);
    fb->drawFilledRect(2+80+80,0,1,20,0xff000000);
    fb->drawFilledRect(2+80+80+80,0,1,20,0xff000000);
    
    if (mActiveMenu)
    {
        pf->drawFilledRect(fb,2+(mActiveMenu - 1) * 80,
                              18,
                              120 + 2+(mActiveMenu - 1) * 80,
                              18 * MenuItems[mActiveMenu - 1] + 18,
                              0xff000000,0xafffffff);
        //fb->drawFilledRect(2+(mActiveMenu - 1) * 80,18,120,18 * MenuItems[mActiveMenu - 1],0xffffffff);
        if (mActiveMenuItem)
            fb->drawFilledRect(3+(mActiveMenu - 1) * 80,19 + 18 * (mActiveMenuItem - 1),118,16,0xffffffff);

        INT32 i;
        for (i = 0; i < MenuItems[mActiveMenu - 1]; i++)
            pf->mPrinter->print(fb,4+(mActiveMenu - 1) * 80,19 + 18 * i, menu[mActiveMenu - 1][i].mTitle,XFCBLEND_MUL_FAST);
    }
}


INT Dropdown::checkHit(INT32 aX, INT32 aY, INT32 aEvent)
{
    PaintForge * pf = PaintForge::getInstance();

    if (aEvent == 1 && Widget::checkHit(aX,aY,aEvent) == 0)
    {
        return 0;
    }
    if (Widget::checkHit(aX,aY,0)) // hit title bar
    {
        mActiveMenuItem = 0; // no active menu item (as we're in the title bar)
        if (mActiveMenu)
            PaintForge::getInstance()->mCanvas->addToDirtyRect(64 + (mActiveMenu - 1) * 80,18,120,MenuItems[mActiveMenu - 1] * 18,PaintForge::getInstance()->mCanvas->mDirtyFrameRect);
        mActiveMenu = ((aX - 2) / 80) + 1;
    }
    else
    if (mActiveMenu)
    {
        mActiveMenuItem = 0; // by default, no active menu item
        INT32 i;
        for (i = 0; i < MenuItems[mActiveMenu - 1]; i++)
            if (aX >  2 + (mActiveMenu - 1) * 80 &&
                aX <= 2 + (mActiveMenu - 1) * 80 + 120 &&
                aY > 18 + (18 * i) &&
                aY <= 18 * 2 + (18 * i))
                mActiveMenuItem = i + 1;
    }
    if (aEvent == -1 && mActiveMenuItem)
    {
        switch (menu[mActiveMenu - 1][mActiveMenuItem - 1].mFunction)
        {
        case MENU_QUIT:
            XFcCore::quit();
            break;
        case MENU_CLEAR:
            PaintForge::getInstance()->mCanvas->effectClear();
            break;
        case MENU_UNDO:
            PaintForge::getInstance()->mCanvas->effectUndo();
            break;
        case MENU_SPLASH:
            {
                pf->mRunLevel = SPLASH_IDLING;
                pf->mGL->getSecondary()->drawImage(pf->mLogo);
            }
            break;
        case MENU_ABOUTPF:
            pf->mActiveWidgets = pf->mHelpPage;
            pf->mHelpPage->init(0);
            break;
        case MENU_LOREMIPSUM:
            pf->mActiveWidgets = pf->mHelpPage;
            pf->mHelpPage->init(1);
            break;
        case MENU_BRUSHMODE:
            PaintForge::getInstance()->mCanvas->effectBrushmode();
            break;
        case MENU_SAVE:
            pf->mActiveWidgets = pf->mFileDialog;
            pf->mFileDialog->init(0);
            break;
        case MENU_LOAD:
            pf->mActiveWidgets = pf->mFileDialog;
            pf->mFileDialog->init(1);
            break;
        
        }
    }
    if (aEvent == -1)
    {
        mActiveMenu = mActiveMenuItem = 0;
        if (mActiveMenu)
            PaintForge::getInstance()->mCanvas->addToDirtyRect(2 + (mActiveMenu - 1) * 80,18,120,MenuItems[mActiveMenu - 1] * 18,PaintForge::getInstance()->mCanvas->mDirtyFrameRect);

//        PaintForge::getInstance()->mCanvas->addToDirtyRect(0,0,480,320,PaintForge::getInstance()->mCanvas->mDirtyFrameRect);
    }
    return 1;    
}
