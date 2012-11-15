#include "lpdrumkit.h"

LPDrumKit::LPDrumKit(LibLaunpad* launchpad)  : LPMode(launchpad)
{
}

void LPDrumKit::enter()
{

    H2Core::Song* song = H2Core::Hydrogen::get_instance()->getSong();
    H2Core::InstrumentList* instruments = song->get_instrument_list();

    int row, col;

    // The kick (the sample we normaly begin with) should be on the lower left corner.
    row = 7;
    col = 0;

    locked = true;
    launchpad->reset();
    launchpad->ctrl(5, LibLaunpad::green_middle);


    if (H2Core::Hydrogen::get_instance()->getState() == STATE_PLAYING)
        launchpad->scene(0, LibLaunpad::green_middle);

    if (H2Core::Preferences::get_instance()->m_bUseMetronome)
        launchpad->scene(1, LibLaunpad::green_low);

    for (int i=0; i<instruments->size(); i++) {
        int color;
        if ((row < 4 && col < 4) || (row > 4 && col > 4)) {
            color = LibLaunpad::red_middle;
        } else {
            color = LibLaunpad::green_middle;
        }
        launchpad->matrix(row, col, color);
        col++;
        if (col > 3) {
            row--;
            col = 0;
            if (row < 0 ) {
                break;
            }
        }
    }

    locked = false;
    ___DEBUGLOG("entered LPPatternEditor.");
}

void LPDrumKit::exit()
{
    ___DEBUGLOG("exited lpmixer.");
}


void LPDrumKit::upPressed(int val)
{

}

void LPDrumKit::downPressed(int val)
{

}
void LPDrumKit::leftPressed(int val)
{

}
void LPDrumKit::rightPressed(int val)
{

}

void LPDrumKit::matrixPressed(LibLaunpad::Button btn)
{
    // Default note values;
    const float velocity = 0.8f;
    const float pan_L = 0.5f;
    const float pan_R = 0.5f;
    const int nLength = -1;
    const float fPitch = 0.0f;



    locked = true;
    if (btn.velocity > 0) {
        int inst_index = (4*(7-btn.row)) + btn.column;

        H2Core::Song* song = H2Core::Hydrogen::get_instance()->getSong();
        if (inst_index > song->get_instrument_list()->size())
            return;
        H2Core::Instrument* pSelectedInstrument = song->get_instrument_list()->get(inst_index);
        H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );
        H2Core::Note *pNote = new H2Core::Note(pSelectedInstrument, 0, velocity, pan_L, pan_R, nLength, fPitch);
        H2Core::AudioEngine::get_instance()->get_sampler()->note_on(pNote);
        btn.velocity = LibLaunpad::green_high;
        H2Core::AudioEngine::get_instance()->unlock();
    } else {
        btn.velocity = LibLaunpad::green_middle;
    }
    launchpad->matrix(btn);
    locked = false;
}

void LPDrumKit::scenePressed(LibLaunpad::Button btn)
{
    if (btn.row == 0) {
        if ( btn.velocity > 0 ) {
            if (H2Core::Hydrogen::get_instance()->getState() == STATE_PLAYING) {
                H2Core::Hydrogen::get_instance()->sequencer_stop();
                btn.velocity = LibLaunpad::red_high;
            } else {
                H2Core::Hydrogen::get_instance()->sequencer_play();
                btn.velocity = LibLaunpad::green_high;
            }

        } else {
            if (H2Core::Hydrogen::get_instance()->getState() == STATE_PLAYING) {
                btn.velocity = LibLaunpad::green_middle;
            } else {
                btn.velocity = 0;
            }
        }
    } else if (btn.row == 1) {
        H2Core::Preferences* pPref = H2Core::Preferences::get_instance();
        if (btn.velocity > 0) {
            if (pPref->m_bUseMetronome) {
                pPref->m_bUseMetronome = false;
                btn.velocity = 0;
                btn.velocity = LibLaunpad::green_middle;
            } else {
                pPref->m_bUseMetronome = true;
                btn.velocity = LibLaunpad::green_high;
            }
        } else {
            if (pPref->m_bUseMetronome) {
                btn.velocity = LibLaunpad::green_low;
            } else {
                btn.velocity = 0;
            }

        }
    }
    launchpad->matrix(btn);
}
