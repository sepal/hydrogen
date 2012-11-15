#ifndef LPPATTERNEDITOR_H
#define LPPATTERNEDITOR_H

#include <hydrogen/hydrogen.h>
#include <hydrogen/audio_engine.h>
#include <hydrogen/basics/note.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/instrument.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/basics/pattern_list.h>
#include <hydrogen/basics/pattern.h>

#include "lpmode.h"

class LPPatternEditor : public LPMode
{
public:
    LPPatternEditor(LibLaunpad* launchpad);

    void enter();
    void exit();

    void upPressed(int val);
    void downPressed(int val);
    void leftPressed(int val);
    void rightPressed(int val);

    void matrixPressed(LibLaunpad::Button btn);
    void scenePressed(LibLaunpad::Button btn);
private:
    int instOffset;
};

#endif // LPPATTERNEDITOR_H
