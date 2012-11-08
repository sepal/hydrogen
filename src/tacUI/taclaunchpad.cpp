#include "taclaunchpad.h"
#include <iostream>

TacLaunchpad* TacLaunchpad::__instance = NULL;
const char* TacLaunchpad::__class_name = "TacLaunchpad";

void TacLaunchpad::create_instance()
{
    if( __instance == 0 ) {
        __instance = new TacLaunchpad;
    }
}

TacLaunchpad::TacLaunchpad()
    : H2Core::Object(__class_name),
      patternEditor(&launchpad),
      currentMode(NULL)
{
    __instance = this;
    INFOLOG( "INIT" );
    launchpad.open();
}

TacLaunchpad::~TacLaunchpad()
{
    launchpad.close();
}

void TacLaunchpad::checkUI()
{
    launchpad.receive();


    LibLaunpad::Button btn = launchpad.receivedButton();


    if (!btn.is_control) {
        if (currentMode == NULL)
            return ;

        if (btn.column == 8) {
            currentMode->scenePressed(btn);
        } else {
            currentMode->matrixPressed(btn);
        }
    } else {
        switch (btn.column) {
        case 0:
            if (currentMode == NULL)
                return ;
            currentMode->upPressed(btn.velocity);
            break;
        case 1:
            if (currentMode == NULL)
                return ;
            currentMode->downPressed(btn.velocity);
            break;
        case 2:
            if (currentMode == NULL)
                return ;
            currentMode->leftPressed(btn.velocity);
            break;
        case 3:
            if (currentMode == NULL)
                return ;
            currentMode->rightPressed(btn.velocity);
            break;
        case 4:
            // Pressing Session while in session mode will exit the program.
            if (currentMode == &patternEditor) {
                this->stop();
            }
            break;
        }
    }
}

void TacLaunchpad::started() {
    launchpad.ctrl(4, LibLaunpad::green_high);
    currentMode = &patternEditor;
}

void TacLaunchpad::stopped() {
    launchpad.reset();
}
