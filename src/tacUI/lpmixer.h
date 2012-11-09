#ifndef LPMIXER_H
#define LPMIXER_H

#include <hydrogen/hydrogen.h>
#include <hydrogen/audio_engine.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/instrument.h>

#include "lpmode.h"

class LPMixer : public LPMode
{
public:
    LPMixer(LibLaunpad* launchpad);

    void enter();
    void exit();

    void upPressed(int val);
    void downPressed(int val);
    void leftPressed(int val);
    void rightPressed(int val);

    void matrixPressed(LibLaunpad::Button btn);
    void scenePressed(LibLaunpad::Button btn);

    void draw();

private:
    // Cached volume
    float vol[8];
    static const float vol_div = 1.0f/8.0f;
};

#endif // LPMIXER_H
