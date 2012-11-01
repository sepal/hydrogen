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

#include <iostream>
using namespace std;

void showInfo();
void showUsage();


int main(int argc, char* argv[])
{
    try {

    }
    catch ( const H2Core::H2Exception& ex ) {
        std::cerr << "[main] Exception: " << ex.what() << std::endl;
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

