#ifndef LPMODE_H
#define LPMODE_H

#include "launchpad/liblaunchpad.h"

class LPMode
{
public:
    LPMode(LibLaunpad* launchpad);

    virtual void upPressed(int val) = 0;
    virtual void downPressed(int val) = 0;
    virtual void leftPressed(int val) = 0;
    virtual void rightPressed(int val) = 0;

    virtual void matrixPressed(LibLaunpad::Button btn) = 0;
    virtual void scenePressed(LibLaunpad::Button btn) = 0;

protected:
    LibLaunpad* launchpad;
};

#endif // LPMODE_H
