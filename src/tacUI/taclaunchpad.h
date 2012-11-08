#ifndef TACLAUNCHPAD_H
#define TACLAUNCHPAD_H

#include <hydrogen/hydrogen.h>
#include <hydrogen/audio_engine.h>
#include <hydrogen/basics/note.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/instrument.h>
#include <hydrogen/basics/pattern_list.h>
#include <hydrogen/basics/pattern.h>
#include <hydrogen/Preferences.h>

#include "tacui.h"
#include "launchpad/liblaunchpad.h"
#include "lppatterneditor.h"

class TacLaunchpad : public H2Core::Object, public TacUI
{
    H2_OBJECT
public:
    static void create_instance();
    static TacLaunchpad* get_instance() { assert(__instance); return __instance; }
    ~TacLaunchpad();

    void checkUI();

protected:
    virtual void started();
    virtual void stopped();

private:
    static TacLaunchpad* __instance;
    LibLaunpad launchpad;

    LPPatternEditor patternEditor;
    LPMode* currentMode;

    TacLaunchpad();

};

#endif // TACLAUNCHPAD_H
