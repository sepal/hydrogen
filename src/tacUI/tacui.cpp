#include "tacui.h"

pthread_t tacUIDriverThread;
pthread_t tacUIDrawingThread;

void* tacUI_processCaller( void* param )
{
    TacUI* tacUI = ( TacUI* ) param;


    while (tacUI->running()) {
        tacUI->checkUI();
    }
}

void* tacUI_drawing_processCaller( void* param )
{
    TacUI* tacUI = ( TacUI* ) param;


    while (tacUI->running()) {
        tacUI->draw();
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
    pthread_create( &tacUIDrawingThread, &attr, tacUI_drawing_processCaller, this );
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
    pthread_join ( tacUIDrawingThread, NULL );
    this->stopped();
}
