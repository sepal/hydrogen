/**
 * Novation Launchpad cpp lib based on the c lib liblaunchpad by unkownen.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * and GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "liblaunchpad.h"

LibLaunpad::LibLaunpad()
{
    iData = NULL;
    oData = NULL;
    received = 0;
    parseAt = 0;
    context = NULL;
    event[0] = NOTE;
}

void LibLaunpad::open()
{
    if (libusb_init(&context) != 0) {
        throw new LibLaunpadException("Unable to initialize usb.");
    }

    device = libusb_open_device_with_vid_pid(context, LP_ID_VENDOR, LP_ID_PRODUCT);
    if (device == NULL) {
        throw new LibLaunpadException("Unable to find launchpad.");
    }

    int result = libusb_claim_interface(device, 0);
    if (result == LIBUSB_ERROR_BUSY) {
        result = libusb_kernel_driver_active(device, 0);
        if (result == 1 && libusb_detach_kernel_driver(device, 0) == 0 && libusb_claim_interface(device, 0) == 0)
        {
            std::cout << "Found launchpad, ready to rock!" << std::endl;
        } else {
            throw new LibLaunpadException("Unable to claim the launchpad.");
        }
    }

    iData = (unsigned char*)malloc(sizeof(unsigned char)*MAX_PACKET_SIZE);
    if (iData == NULL) {
        throw new LibLaunpadException("Unable to allocate the input buffer.");
    }

    oData = (unsigned char*)malloc(sizeof(unsigned char)*MAX_PACKET_SIZE);
    if (oData == NULL) {
        throw new LibLaunpadException("Unable to allocate the output buffer.");
    }

    reset();
}

void LibLaunpad::close()
{
    libusb_release_interface(device, 0);
    libusb_close(device);
    free(oData);
    free(iData);

    libusb_exit(context);
}

void LibLaunpad::receive()
{
    if (parseAt == received) {
        // No data to parse, lets wait for some.
        parseAt = 0;
        received = 0;
        libusb_interrupt_transfer(device, EP_IN, iData, MAX_PACKET_SIZE, &received, 0);
    }

    // check if the first byte is a prefix byte
    if (iData[parseAt] == NOTE || iData[parseAt] == CTRL) {
        event[0] = iData[parseAt++];
    }
    event[1] = iData[parseAt++];
    event[2] = iData[parseAt++];
}

int LibLaunpad::send(int size)
{
    int transmitted;

    libusb_interrupt_transfer(device, EP_OUT, oData, size, &transmitted, 0);

    return transmitted;
}

int LibLaunpad::send3(unsigned int data0, unsigned int data1, unsigned int data2)
{
    oData[0] = data0;
    oData[1] = data1;
    oData[2] = data2;
    return send(3);
}

int LibLaunpad::receivedType() {
    return event[0];
}

int LibLaunpad::receivedNum() {
    return event[1];
}

int LibLaunpad::receivedVal() {
    return event[2];
}

LibLaunpad::Button LibLaunpad::receivedButton()
{
    int y = event[1] / 16;
    int x = event[1] - (16 * y);

    return Button(receivedType() == CTRL ? true : false, x, y, event[2]);
}

int LibLaunpad::check(int intensity)
{
    intensity = intensity > 2 ? 2 : intensity;
    intensity = intensity < 1 ? 1 : intensity;

    return send3(CTRL, 0, 124+intensity);
}

int LibLaunpad::reset()
{
    return send3(CTRL, 0, 0);
}

int LibLaunpad::setMode(buffer displaying, buffer updating, bool flashing, bool copy)
{
    int v = 32;
    v += displaying;
    v += updating * 4;
    v += flashing * 8;
    v += copy * 16;

    return send3(CTRL, 0, v);
}

int LibLaunpad::matrix(int row, int col, int velocity)
{
    if (row < 0 || row > 7 || col < 0 || col > 8) {
        return 0;
    }
    return send3(NOTE, row * 16 + col, velocity);
}


int LibLaunpad::matrix(Button b)
{
    if (!b.is_control) {
        return this->matrix(b.row, b.column, b.velocity);
    } else {
        return -1;
    }
}

int LibLaunpad::scene(int row, int velocity)
{
    if (row < 0 || row > 7)
        return 0;

    return send3(NOTE, row*16 + 8, velocity);
}

int LibLaunpad::ctrl(int col, int velocity)
{
    if (col < 0 || col > 7)
        return 0;

    return send3(CTRL, 104+col, velocity);
}
