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

#include "taclaunchpad.h"
#include <qt4/QtCore/QString>
#include "hydrogen/playlist.h"

#include <iostream>
using namespace std;

void showInfo();
void showUsage();


int main(int argc, char* argv[])
{
    try {
        const char* logLevelOpt = "Debug";

        H2Core::Logger::create_instance();
        H2Core::Logger::set_bit_mask( H2Core::Logger::parse_log_level( logLevelOpt ) );
        H2Core::Logger* logger = H2Core::Logger::get_instance();
        H2Core::Object::bootstrap( logger, logger->should_log(H2Core::Logger::Debug) );

        H2Core::Filesystem::bootstrap( logger, "./data" );

        MidiMap::create_instance();
        H2Core::Preferences::create_instance();

        ___INFOLOG( QString("Using QT version ") + QString( qVersion() ) );
        ___INFOLOG( "Using data path: " + H2Core::Filesystem::sys_data_path() );

        H2Core::Preferences *pPref = H2Core::Preferences::get_instance();

        pPref->m_sAudioDriver = "Alsa";

        // Hydrogen here to honor all preferences.
        H2Core::Hydrogen::create_instance();

        // Start with an empty song.
        H2Core::Song *song = H2Core::Song::get_empty_song();
        song->set_filename("");

        H2Core::Hydrogen::get_instance()->setSong(song);

        TacLaunchpad::create_instance();

        TacLaunchpad::get_instance()->start();

        while (TacLaunchpad::get_instance()->running());

        TacLaunchpad::get_instance()->stop();

        // Clean up.
        /*H2Core::PatternList* patterns = song->get_pattern_list();
        for (int i=0; i<patterns->size(); i++) {
            H2Core::Pattern* pattern = patterns->get(i);
            if (pattern != NULL) {
                std::cout << "Deleting " << pattern->get_name().toStdString().c_str() << endl;
                delete pattern;
            }
        }*/

        delete song;

        delete TacLaunchpad::get_instance();
        delete H2Core::Hydrogen::get_instance();

        delete Playlist::get_instance();
        delete H2Core::Preferences::get_instance();
        delete H2Core::EventQueue::get_instance();
        delete H2Core::AudioEngine::get_instance();

        delete MidiMap::get_instance();
        delete MidiActionManager::get_instance();

        ___INFOLOG( "Quitting..." );
        cout << "\nBye..." << endl;
        delete H2Core::Logger::get_instance();

        if (H2Core::Object::count_active()) {
            H2Core::Object::write_objects_map_to_cerr();
        }
    }
    catch ( const H2Core::H2Exception& ex ) {
        std::cerr << "[main] Exception: " << ex.what() << std::endl;
    }
    catch (LibLaunpadException *e) {
        std::cerr << "[main] Launchpad exception: " << e->what() << std::endl;
    }
    catch (...) {
        std::cerr << "[main] Unknown exception X" << std::endl;
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

