//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef FILEDIALOG_H_INCLUDED
#define FILEDIALOG_H_INCLUDED

class FileDialog : public Widget
{
public:
    FileDialog();
    virtual void render();
    virtual INT checkHit(INT32 aX, INT32 aY, INT32 aEvent);
    INT mDialogMode;
    CHAR *mCurrentFilename;
    void onCharEntered(INT32 aChar);
    void init(INT32 aMode);
    void deinit();
    CHAR **mFiles;
    INT32 mFileCount;
    INT32 mFileOffset;
    INT load();
    INT save();
    INT32 mActiveWidget;
    INT mRefresh;
};

#endif // PFWIDGET_H_INCLUDED