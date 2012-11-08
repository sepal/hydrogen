#ifndef LPMODE_H
#define LPMODE_H

#include "launchpad/liblaunchpad.h"

class LPMode
{
public:
    LPMode(LibLaunpad* launchpad);

    virtual void enter() = 0;
    virtual void exit() = 0;

    virtual void upPressed(int val) = 0;
    virtual void downPressed(int val) = 0;
    virtual void leftPressed(int val) = 0;
    virtual void rightPressed(int val) = 0;

    virtual void matrixPressed(LibLaunpad::Button btn) = 0;
    virtual void scenePressed(LibLaunpad::Button btn) = 0;
    virtual void draw() = 0;

    void lock();
    bool try_lock();
    void unlock();
protected:
    LibLaunpad* launchpad;

private:
    /// Mutex for syncronized access to the launchpad.
    pthread_mutex_t __launchpad_mutex;
};

#endif // LPMODE_H
