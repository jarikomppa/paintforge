//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef HELPPAGE_H_INCLUDED
#define HELPPAGE_H_INCLUDED

class HelpPage : public Widget
{
public:
    HelpPage();
    virtual void render();
    virtual INT checkHit(INT32 aX, INT32 aY, INT32 aEvent);
    void init(INT32 aMode);
    void deinit();
    INT32 mDialogMode;
    CHAR *mText;
    INT32 mLineCount;
    INT32 mLineOffset;
    INT32 mActiveWidget;
    INT mRefresh;
};

#endif // PFWIDGET_H_INCLUDED