#include "lppatterneditor.h"

LPPatternEditor::LPPatternEditor(LibLaunpad* launchpad) : LPMode(launchpad)
{
    instOffset = true;
}

void LPPatternEditor::enter()
{
    launchpad->reset();
    launchpad->ctrl(4, LibLaunpad::green_middle);
    H2Core::Song* song = H2Core::Hydrogen::get_instance()->getSong();
    H2Core::Pattern* pPattern = song->get_pattern_list()->get(0);

    locked = true;

    if (H2Core::Hydrogen::get_instance()->getState() == STATE_PLAYING)
        launchpad->scene(0, LibLaunpad::green_middle);

    if (H2Core::Preferences::get_instance()->m_bUseMetronome)
        launchpad->scene(1, LibLaunpad::green_low);

    const H2Core::Pattern::notes_t* notes = pPattern->get_notes();
    for (H2Core::Pattern::notes_cst_it_t it = notes->begin(); it != notes->end(); it++) {
        H2Core::Note note = (*it).second;
        launchpad->matrix(note.get_instrument()->get_id(), note.get_position()/24, LibLaunpad::green_middle);
    }
    std::cout << std::endl;
    locked = false;


    ___DEBUGLOG("entered LPPatternEditor.");
}

void LPPatternEditor::exit()
{
    ___DEBUGLOG("exited lpmixer.");
}

void LPPatternEditor::upPressed(int val)
{
    if ( val>0 ) {

    } else {

    }
}

void LPPatternEditor::downPressed(int val)
{
}

void LPPatternEditor::leftPressed(int val)
{
}

void LPPatternEditor::rightPressed(int val)
{
}

void LPPatternEditor::matrixPressed(LibLaunpad::Button btn)
{
    // Default note values;
    const float velocity = 0.8f;
    const float pan_L = 0.5f;
    const float pan_R = 0.5f;
    const int nLength = -1;
    const float fPitch = 0.0f;

    H2Core::Song* song = H2Core::Hydrogen::get_instance()->getSong();
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
            btn.velocity = LibLaunpad::green_high;
        } else {
            // Delete old note.
            pPattern->remove_note(pOldNote);
            delete pOldNote;
            btn.velocity = 0;
        }

        H2Core::AudioEngine::get_instance()->unlock();
        // Update the pressed launchpad button.
        launchpad->matrix(btn);
    }

}

void LPPatternEditor::scenePressed(LibLaunpad::Button btn)
{
    // Default note values;
    /*const float velocity = 0.8f;
    const float pan_L = 0.5f;
    const float pan_R = 0.5f;
    const int nLength = -1;
    const float fPitch = 0.0f;


    if (btn.velocity > 0) {
        H2Core::Song* song = H2Core::Hydrogen::get_instance()->getSong();
        H2Core::Instrument* pSelectedInstrument = song->get_instrument_list()->get(btn.row);

        H2Core::AudioEngine::get_instance()->lock( RIGHT_HERE );	// lock the audio engine
        H2Core::Note *pNote = new H2Core::Note(pSelectedInstrument, 0, velocity, pan_L, pan_R, nLength, fPitch);
        H2Core::AudioEngine::get_instance()->get_sampler()->note_on(pNote);
        btn.velocity = LibLaunpad::green_high;
        H2Core::AudioEngine::get_instance()->unlock();
    } else {
        btn.velocity = 0;
    }
    launchpad->matrix(btn);*/
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
