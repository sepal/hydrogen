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
      drumkit(&launchpad),
      mixer(&launchpad),
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
        if (currentMode != NULL) {
            if (btn.column == 8) {
                currentMode->scenePressed(btn);
            } else {
                currentMode->matrixPressed(btn);
            }
        }
    } else {
        switch (btn.column) {
        case 0:
            if (currentMode != NULL)
                currentMode->upPressed(btn.velocity);
            break;
        case 1:
            if (currentMode == NULL)
                currentMode->downPressed(btn.velocity);
            break;
        case 2:
            if (currentMode == NULL)
                currentMode->leftPressed(btn.velocity);
            break;
        case 3:
            if (currentMode == NULL)
                currentMode->rightPressed(btn.velocity);
            break;
        case 4:
            // Pressing session while in session mode will exit the program.
            if (currentMode == &patternEditor && btn.velocity > 0) {
                this->stop();
            } else if(btn.velocity > 0) {
                switchMode(&patternEditor);
            }
            break;
        case 5:
            if (btn.velocity > 0)
                switchMode(&drumkit);
            break;
        case 7:
            if (btn.velocity > 0)
                switchMode(&mixer);
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

void TacLaunchpad::draw() {
    if (currentMode) {
        currentMode->draw();
    }
}

void TacLaunchpad::switchMode(LPMode* newMode)
{
    if (newMode && newMode != currentMode) {
        if (currentMode)
            currentMode->exit();
        currentMode = newMode;
        currentMode->enter();
    }
}
