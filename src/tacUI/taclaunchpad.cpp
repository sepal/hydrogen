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
    : H2Core::Object(__class_name)
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

    H2Core::Song* song = H2Core::Hydrogen::get_instance()->getSong();

    if (!btn.is_control) {
        // Default note values;
        const float velocity = 0.8f;
        const float pan_L = 0.5f;
        const float pan_R = 0.5f;
        const int nLength = -1;
        const float fPitch = 0.0f;

        if (btn.column == 8) {
            H2Core::Instrument* pSelectedInstrument = song->get_instrument_list()->get(btn.row);


            H2Core::Note *pNote = new H2Core::Note(pSelectedInstrument, 0, velocity, pan_L, pan_R, nLength, fPitch);

            if (btn.velocity > 0) {
                H2Core::AudioEngine::get_instance()->get_sampler()->note_on(pNote);
                btn.velocity = LibLaunpad::green_high;
            } else {
                btn.velocity = 0;
            }
            H2Core::AudioEngine::get_instance()->unlock();
            launchpad.matrix(btn);
        } else {
            // We'll only work with the first pattern for now.
            if (btn.velocity > 0) {
                H2Core::Pattern* pPattern = song->get_pattern_list()->get(0);
                H2Core::Instrument* pInstrument = song->get_instrument_list()->get( btn.row );


                H2Core::AudioEngine::get_instance()->lock(RIGHT_HERE);

                // We'll work with 1/8ths for now there for the *24.
                int column = btn.column*24;
                H2Core::Note* pOldNote = pPattern->find_note(column, -1, pInstrument);

                if ( pOldNote == NULL ) { //!bNoteAlreadyExist) {
                    // Add a new note, if there wasn't one.
                    H2Core::Note* pNewNote = new H2Core::Note(pInstrument, column, velocity, pan_L, pan_R, nLength, fPitch);
                    pPattern->insert_note(pNewNote);
                    btn.velocity = LibLaunpad::green_middle;
                } else {
                    // Delete old note.
                    pPattern->remove_note(pOldNote);
                    delete pOldNote;
                    btn.velocity = 0;
                }

                H2Core::AudioEngine::get_instance()->unlock();
                // Update the pressed launchpad button.
                launchpad.matrix(btn);
            }

        }
    } else {
        switch (launchpad.receivedNum()) {
        case 104:
            this->stop();
            break;
        case 108:
            if ( launchpad.receivedVal() > 0 ) {
                H2Core::Hydrogen* pEngine = H2Core::Hydrogen::get_instance();
                if (pEngine->getState() == STATE_PLAYING) {
                    pEngine->sequencer_stop();
                    launchpad.ctrl(4, 0);
                } else if (pEngine->getState() == STATE_READY) {
                    pEngine->sequencer_play();
                    launchpad.ctrl(4, LibLaunpad::green_high);
                }
            }
            break;
        case 109:
            if ( launchpad.receivedVal() > 0 ) {
                H2Core::Preferences* pPref = H2Core::Preferences::get_instance();
                pPref->m_bUseMetronome = pPref->m_bUseMetronome == true ? false : true;
                launchpad.ctrl(5, (pPref->m_bUseMetronome ? LibLaunpad::green_middle : 0));
            }
            break;
        }
    }
}

void TacLaunchpad::started() {
    launchpad.ctrl(0, LibLaunpad::green_high);
}

void TacLaunchpad::stopped() {
    launchpad.reset();
}
