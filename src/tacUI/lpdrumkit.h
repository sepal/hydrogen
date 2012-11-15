#ifndef LPDRUMKKIT_H
#define LPDRUMKKIT_H

#include <hydrogen/hydrogen.h>
#include <hydrogen/audio_engine.h>
#include <hydrogen/basics/note.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/instrument.h>
#include <hydrogen/Preferences.h>

#include "lpmode.h"

class LPDrumKit : public LPMode
{
public:
    LPDrumKit(LibLaunpad* launchpad);

    void enter();
    void exit();

    virtual void upPressed(int val);
    virtual void downPressed(int val);
    virtual void leftPressed(int val);
    virtual void rightPressed(int val);

    virtual void matrixPressed(LibLaunpad::Button btn);
    virtual void scenePressed(LibLaunpad::Button btn);
};

#endif // LPDRUMKKIT_H
