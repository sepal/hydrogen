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


#ifndef LIBLAUNCHPAD_H
#define LIBLAUNCHPAD_H

#include <stdlib.h>
#include <exception>
#include <libusb-1.0/libusb.h>
#include <iostream>

// launchpad identifiers
#define LP_ID_VENDOR  0x1235
#define LP_ID_PRODUCT 0x000e

// size to use for buffers
#define MAX_PACKET_SIZE 8

// usb endpoints
#define EP_IN      ( LIBUSB_ENDPOINT_IN  | 1)
#define EP_OUT     ( LIBUSB_ENDPOINT_OUT | 2)

// midi channels used for communication
#define CTRL 0xB0 // control change, channel 1
#define NOTE 0x90 // note on, channel 1
#define NOTE_ON 0x90 // note on, channel 1
#define NOTE_OFF 0x80 // note off, channel 1

class LibLaunpad
{
public:
    struct Button {
        Button (bool is_cc, int x_pos, int y_pos, int vel) :
            is_control(is_cc), column(x_pos), row(y_pos), velocity(vel)
        {}
        bool is_control;
        int column;
        int row;
        int velocity;
    };

    /**
     * Buffer identifiers, the launchpad has to two buffers.
     */
    enum buffer {
        buffer0 = 0,
        buffer1 = 1
    };

    /**
     * Different intensities of red.
     */
    enum red {
        red_low     = 0x01,
        red_middle  = 0x02,
        red_high    = 0x03
    };

    /**
     * Different intensities of green.
     */
    enum green {
        green_low    = 0x10,
        green_middle = 0x20,
        green_high   = 0x30
    };

    /**
     * The leds buffer setting modes.
     */
    enum ledMode {
        copy =  0x0C,
        clear = 0x08
    };

    LibLaunpad();

    /**
     * Reguster the launchpad.
     */
    void open();
    /**
     * Deregister the launchpad.
     */
    void close();

    /**
     * Receive data from the launchpad.
     *
     * This is a blocking function waiting for some data to arrive to the launchpad. The actual data
     * can be retrieved via the functions receivedType, receivedNum and receivedVal. Received data
     * is stored inside launchpad->rdata, then parsed as a midi event in launchpad->event. Sometimes
     * several events are received at once. In such a case, only the first event is parsed. The next
     * time the function is called, the next event in the buffer is parsed until the buffer is empty.
     */
    void receive();

    /**
     * Send data to the launchpad.
     *
     * @param size:
     *     Amount of data to send.
     */
    int send(int size);

    /**
     * Send a standard three bytes message.
     *
     * @param data0:
     *     First byte.
     *
     * @param data1:
     *     Second byte.
     *
     * @param data2:
     *     Third byte.
     */
    int send3(unsigned int data0, unsigned int data1, unsigned int data2);

    /**
     * Returns the type of the received pressed button.
     *
     * @return
     *     Either NOTE (0x90) for one of a note, ie the note and scene buttons or CTRL (0xB0) for
     *     the top row automap functions.
     */
    int receivedType();

    /**
     * Returns the number of the pressed button.
     *
     * @return
     *     Returns a number defining either a note or a control change (button) number.
     */
    int receivedNum();

    /**
     * Returns the value of the pressed button.
     *
     * @return
     *     The velocity or the (cc) value of the pressed button.
     */
    int receivedVal();

    /**
     * Returns the last retreived button.
     *
     * @return
     *     Returns a struct Button, which defines it button in a matrix form.
     */
    Button receivedButton();

    /**
     * Turns all the leds on.
     *
     * @param intensity:
     *     A value between 1 and 3 that defines the intensity of the leds.
     *
     */
    int check(int intensity);

    /**
     * Resets the launchpad.
     */
    int reset();

    /**
     * Sets the launchpads mode.
     *
     * @param displaying:
     *     The buffer to display.
     *
     * @param updating:
     *     The buffer to update.
     *
     * @param flashing:
     *     Wheter the launchpad should display both buffers alternatively.
     *
     * @param copy:
     *     Whether we should copy the content of the newly displaying buffer into the newly updating buffer.
     */
    int setMode(buffer displaying, buffer updating, bool flashing, bool copy);

    /**
     * Set the velocity for the matrix.
     */
    int matrix(int row, int col, int velocity);

    /**
     * Set the a button using the button struct.
     */
    int matrix(Button b);

    /**
     * Set the velocity for an entire row ie. a scene.
     */
    int scene(int row, int velocity);

    /**
     * Set the velocity for the top row/automap buttons(they use cc messages).
     */
    int ctrl(int col, int velocity);
private:
    /**
     * Device handle.
     */
    libusb_device_handle* device;

    /**
     * Incomming data buffer.
     */
    unsigned char* iData;

    /**
     * Outgoing data buffer.
     */
    unsigned char* oData;

    /**
     * Amount of data currently stored in iData;
     */
    int received;

    /**
     * Where to read in the oData to get the next event.
     */
    int parseAt;

    /**
     * Store the parsed midi event.
     */
    int event[3];

    /**
     * Libusb context, in case whe have more then one launchpad.
     */
    libusb_context* context;
};

class LibLaunpadException : public std::exception
{
public:
    LibLaunpadException(const char* msg)
    {
        this->msg = msg;
    }

    virtual const char* what() const throw()
  {
    return msg;
  }

private:
    const char* msg;
};

#endif // LIBLAUNCHPAD_H
