#include "lpmixer.h"

LPMixer::LPMixer(LibLaunpad* launchpad) : LPMode(launchpad)
{
}

void LPMixer::enter()
{
    launchpad->reset();
    for (int i=0; i<8; i++) {
        vol[i] = 0;
    }
    for (int x=0; x<8; x++) {
        for (int y=0; y<8; y++) {
            pressed[x][y] = false;
        }
    }
    launchpad->ctrl(7, LibLaunpad::green_middle);
    ___DEBUGLOG("entered lpmixer.");
}

void LPMixer::exit()
{
    ___DEBUGLOG("exited lpmixer.");
}

void LPMixer::upPressed(int val)
{
}

void LPMixer::downPressed(int val)
{
}

void LPMixer::leftPressed(int val)
{
}

void LPMixer::rightPressed(int val)
{
}

void LPMixer::matrixPressed(LibLaunpad::Button btn)
{
    if (btn.velocity > 0) {
        pressed[btn.row][btn.column] = true;
        H2Core::Instrument* instrument = H2Core::Hydrogen::get_instance()->getSong()->get_instrument_list()->get(btn.column);
        instrument->set_volume( (7-btn.row) * vol_div );

        if (btn.row == 0) {
            btn.velocity = LibLaunpad::red_high;
        } else {
            btn.velocity = LibLaunpad::green_high;
        }
        launchpad->matrix(btn);
    } else {
        pressed[btn.row][btn.column] = false;
        if (btn.row == 0) {
            btn.velocity = LibLaunpad::red_low;
        } else {
            btn.velocity = LibLaunpad::green_low;
        }
        launchpad->matrix(btn);

    }
}

void LPMixer::scenePressed(LibLaunpad::Button btn)
{
}

void LPMixer::draw()
{
    H2Core::InstrumentList* instruments = H2Core::Hydrogen::get_instance()->getSong()->get_instrument_list();

    for (int i=0; i<8; i++) {
        H2Core::Instrument* instrument = instruments->get(i);
        if (vol[i] != instrument->get_volume()) {
            vol[i] = instrument->get_volume();
            int btn_vol = vol[i] / vol_div;
            std::cout << vol[i] / vol_div << std::endl;
            std::cout <<  btn_vol  << std::endl;

            for (int row=0; row<8; row++) {
                int color = 0;
                if (btn_vol >= row) {
                    color = LibLaunpad::green_low;
                    if (row==7) {
                        color = LibLaunpad::red_low;
                    }
                }
                if (!pressed[7-row][i])
                    launchpad->matrix(7-row, i, color);
            }
        }
    }
}
