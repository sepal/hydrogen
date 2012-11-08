#include "lpmode.h"

LPMode::LPMode(LibLaunpad* launchpad)
{
    this->launchpad = launchpad;
}


void LPMode::lock()
{
    pthread_mutex_lock( &__launchpad_mutex );
}



bool LPMode::try_lock()
{
    int res = pthread_mutex_trylock( &__launchpad_mutex );
    if ( res != 0 ) {
        // Lock not obtained
        return false;
    }
    return true;
}



void LPMode::unlock()
{
    // Leave "__locker" dirty.
    pthread_mutex_unlock( &__launchpad_mutex );
}
