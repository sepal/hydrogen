#include "tacui.h"

pthread_t tacUIDriverThread;

void* tacUI_processCaller( void* param )
{
    TacUI* tacUI = ( TacUI* ) param;


    while (tacUI->running()) {
        tacUI->checkUI();
    }
}

TacUI::TacUI()
{
    mRunning = false;
    pthread_mutex_init( &__engine_mutex, NULL );

}

TacUI::~TacUI()
{
}

void TacUI::start()
{
    pthread_attr_t attr;
    pthread_attr_init( &attr );
    this->mRunning = true;
    pthread_create( &tacUIDriverThread, &attr, tacUI_processCaller, this );
    this->started();
}


bool TacUI::running()
{
    return this->mRunning;
}

void TacUI::stop()
{
    this->mRunning = false;
    pthread_join ( tacUIDriverThread, NULL );
    this->stopped();
}
