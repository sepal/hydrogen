/*
 * GUI-less hydrogen for the use in embedded systems.
 * Copyright(c) 2012 by Sebastian Gilits
 *
 * http://www.hydrogen-music.org
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY, without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */

#include <QLibraryInfo>
#include <hydrogen/config.h>
#include <hydrogen/version.h>
#include <getopt.h>



#ifdef H2CORE_HAVE_LASH
#include <hydrogen/LashClient.h>
#endif

#include <hydrogen/basics/song.h>
#include <hydrogen/midi_map.h>
#include <hydrogen/audio_engine.h>
#include <hydrogen/hydrogen.h>
#include <hydrogen/globals.h>
#include <hydrogen/event_queue.h>
#include <hydrogen/Preferences.h>
#include <hydrogen/h2_exception.h>
#include <hydrogen/helpers/filesystem.h>
#include <hydrogen/basics/pattern_list.h>
#include <hydrogen/basics/pattern.h>
#include <hydrogen/basics/note.h>
#include <hydrogen/basics/instrument_list.h>
#include <hydrogen/basics/instrument.h>

#include "launchpad/liblaunchpad.h"

#include <qt4/QtCore/QString>

#include <iostream>
using namespace std;

void showInfo();
void showUsage();


int main(int argc, char* argv[])
{

    LibLaunpad lp;

    try {
        // Setup utilities.
        const char* logLevelOpt = "Debug";
        H2Core::Logger* logger = H2Core::Logger::bootstrap( H2Core::Logger::parse_log_level( logLevelOpt ) );
        H2Core::Object::bootstrap(logger, logger->should_log( H2Core::Logger::Debug ) );
        // The hydrogen lib should be in the folder data which is in the same as the executable
        H2Core::Filesystem::bootstrap(logger, "./data");
        //MidiMap::create_instance();
        H2Core::Preferences::create_instance();

        ___INFOLOG( QString("Using QT version ") + QString( qVersion() ) );
        ___INFOLOG( "Using data path: " + H2Core::Filesystem::sys_data_path() );

        ___INFOLOG( "Opening Launchpad" );
        lp.open();


        H2Core::Preferences *pPref = H2Core::Preferences::get_instance();
        pPref->loadPreferences(true);

        ___INFOLOG("Setting default settings.");
        pPref->m_sAudioDriver = "Alsa";
        //pPref->m_sMidiDriver

        H2Core::Hydrogen::create_instance();

        H2Core::Song *song = H2Core::Song::get_empty_song();
        song->set_filename("");

        H2Core::Hydrogen::get_instance()->setSong(song);

        ___INFOLOG("Running main thread");
        bool exit = false;
        pPref->m_bUseMetronome = false;

        lp.ctrl(0, LibLaunpad::green_high);

        while(!exit){
            lp.receive();
            LibLaunpad::Button btn = lp.receivedButton();
            if (!btn.is_control) {
                // Default note values;
                const float velocity = 0.8f;
                const float pan_L = 0.5f;
                const float pan_R = 0.5f;
                const int nLength = -1;
                const float fPitch = 0.0f;

                if (btn.column == 8) {
                    H2Core::AudioEngine::get_instance()->lock(RIGHT_HERE);
                    H2Core::Instrument* pSelectedInstrument = song->get_instrument_list()->get(btn.row);


                    H2Core::Note *pNote = new H2Core::Note(pSelectedInstrument, 0, velocity, pan_L, pan_R, nLength, fPitch);

                    if (btn.velocity > 0) {
                        H2Core::AudioEngine::get_instance()->get_sampler()->note_on(pNote);
                        btn.velocity = LibLaunpad::green_high;
                    } else {
                        btn.velocity = 0;
                    }
                    H2Core::AudioEngine::get_instance()->unlock();
                    lp.matrix(btn);
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
                        // Update the pressed lp button.
                        lp.matrix(btn);
                    }

                }
            } else {
                switch (lp.receivedNum()) {
                case 104:
                    exit = true;
                    break;
                case 108:
                    if ( lp.receivedVal() > 0 ) {
                        H2Core::Hydrogen* pEngine = H2Core::Hydrogen::get_instance();
                        if (pEngine->getState() == STATE_PLAYING) {
                            pEngine->sequencer_stop();
                            lp.ctrl(4, 0);
                        } else if (pEngine->getState() == STATE_READY) {
                            pEngine->sequencer_play();
                            lp.ctrl(4, LibLaunpad::green_high);
                        }
                    }
                    break;
                case 109:
                    if ( lp.receivedVal() > 0 ) {
                        pPref->m_bUseMetronome = pPref->m_bUseMetronome == true ? false : true;
                        lp.ctrl(5, (pPref->m_bUseMetronome ? LibLaunpad::green_middle : 0));
                    }
                    break;
                }
            }
        }

        lp.reset();
        lp.close();

        // Cleanup
        delete pPref;
        delete H2Core::EventQueue::get_instance();
        delete H2Core::AudioEngine::get_instance();

        //delete MidiMap::get_instance();
        //delete MidiActionManager::get_instance();

        delete logger;
    }
    catch ( const H2Core::H2Exception& ex ) {
        std::cerr << "[main] Exception: " << ex.what() << std::endl;
    }
    catch (LibLaunpadException *e) {
        std::cerr << "[main] Launchpad exception: " << e->what() << std::endl;
    }
    catch (...) {
        std::cerr << "[main] Unknown exception X-(" << std::endl;
    }

    return 0;
}


/**
 * Show some information
 */
void showInfo()
{
        cout << "\nHydrogen " + H2Core::get_version() + " [" + __DATE__ + "]  [http://www.hydrogen-music.org]" << endl;
        cout << "Copyright 2002-2008 Alessandro Cominu" << endl;
//	_INFOLOG( "Compiled modules: " + QString(COMPILED_FEATURES) << endl;

        if ( H2Core::Object::count_active() ) {
                cout << "\nObject counting active" << endl;
        }

        cout << "\nHydrogen comes with ABSOLUTELY NO WARRANTY" << endl;
        cout << "This is free software, and you are welcome to redistribute it" << endl;
        cout << "under certain conditions. See the file COPYING for details\n" << endl;
}



/**
 * Show the correct usage
 */
void showUsage()
{
}

