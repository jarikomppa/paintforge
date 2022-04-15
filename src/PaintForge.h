//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef PAINTFORGE_H_INCLUDED
#define PAINTFORGE_H_INCLUDED

extern INT32 hsv2rgb(INT32 aHSV);
extern INT32 rgb2hsv(INT32 aRGB);


//#define CHECKER_DARK 0x7f7f7f
//#define CHECKER_LIGHT 0x8f8f8f

#define CHECKER_DARK 0xbfbfbf
#define CHECKER_LIGHT 0xffffff

#define BACKTILE_SIZE 16
#undef MIN
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define SPLASH_IDLING 2

class Widget;
class Canvas;
class XFuPrinter;
class XFuFPSCount;
class FileDialog;
class HelpPage;

class PaintForge : public XFcApp, public XFcInput, public XFcRenderer
{
protected:
    // Inherited from interface classes

    virtual void onPointerDown(INT32 aX, INT32 aY); 
    virtual void onPointerMove(INT32 aX, INT32 aY); 
    virtual void onPointerUp(INT32 aX, INT32 aY); 
    virtual void onControlDown(INT32 aCode);
    virtual void onControlUp(INT32 aCode);
    virtual void onCharEntered(CHAR aChar);
    virtual void render();
    virtual void onAppInit();
    virtual void onJoystickMoved(INT32 aX, INT32 aY);

public:

    PaintForge();

    static PaintForge * getInstance()
    {
        return (PaintForge *)XFcCore::getApp();
    }

    static void drawRect(XFcGLSurface *aSurf, INT32 aX1, INT32 aY1, INT32 aX2, INT32 aY2, INT32 aColor);
    static void drawFilledRect(XFcGLSurface *aSurf, INT32 aX1, INT32 aY1, INT32 aX2, INT32 aY2, INT32 aColor, INT32 aFillColor);

    XFcGLSurface *mLogo;
    XFcGL *mGL;
    FileDialog *mFileDialog;
    HelpPage *mHelpPage;
    Widget *mActiveWidgets;
    Widget *mPaletteWidgets;
    Widget *mCaptured;  // pointer capture
    Canvas *mCanvas;
    XFuPrinter *mPrinter;
    INT32 mRunLevel;
    XFuFPSCount *mFPS;
    INT32 mDoneSplash;
    INT32 mSwatches[50];
    INT mTemporaryPalette; // flag, whether canvas should be returned on fire1 release
    INT32 mKeyDownBuffer[32];
};

#endif //PAINTFORGE_H_INCLUDED