#include "lppatterneditor.h"

LPPatternEditor::LPPatternEditor(LibLaunpad* launchpad) : LPMode(launchpad)
{
}

void LPPatternEditor::upPressed(int val)
{
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
            btn.velocity = LibLaunpad::green_middle;
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
    const float velocity = 0.8f;
    const float pan_L = 0.5f;
    const float pan_R = 0.5f;
    const int nLength = -1;
    const float fPitch = 0.0f;

    H2Core::Song* song = H2Core::Hydrogen::get_instance()->getSong();
    H2Core::Instrument* pSelectedInstrument = song->get_instrument_list()->get(btn.row);

    H2Core::Note *pNote = new H2Core::Note(pSelectedInstrument, 0, velocity, pan_L, pan_R, nLength, fPitch);

    if (btn.velocity > 0) {
        H2Core::AudioEngine::get_instance()->get_sampler()->note_on(pNote);
        btn.velocity = LibLaunpad::green_high;
    } else {
        btn.velocity = 0;
    }
    H2Core::AudioEngine::get_instance()->unlock();
    launchpad->matrix(btn);
}
