#ifndef TACUI_H
#define TACUI_H

#include <hydrogen/object.h>

#include <pthread.h>


/**
* @class TacUI
*
* @brief Base class for tacticle user interfaces.
*
*
* The TacUI class handles all UI thread related stuff, like
* starting and stopping the it, and also a function to check
* if the program is still running.
*
*
* @author Sebastian Gilits
*
*/
class TacUI
{
public:
    ~TacUI();

    void start();
    bool running();
    void stop();
    virtual void checkUI() = 0;
    virtual void draw() = 0;
protected:

    /// Mutex for syncronized access.
    pthread_mutex_t __engine_mutex;
    bool mRunning;

    virtual void started() = 0;
    virtual void stopped() = 0;
    TacUI();
};

#endif // TACUI_H
