//
// PaintForge
// Copyright 2003-2004 Jari Komppa
//

#ifndef PALETTEBACKGROUND_H_INCLUDED
#define PALETTEBACKGROUND_H_INCLUDED

class PaletteBackground : public Widget
{
public:
    virtual void render();
    void init();
    XFcGLSurface * mBackground;
};

#endif // PFPALETTEBACKGROUND_H_INCLUDED