/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#ifndef JOYSTICK_H_
#define JOYSTICK_H_

#include "PS2X_lib/PS2X_lib.h"


class Joystick
{
public:
    Joystick();
    bool init();
    void check_command();

private:
    void _check();

    uint8_t _error;
    byte _type;
    PS2X _ps2x;
    uint8_t _buttons[8];    // left, right, up, down, square, triangle, circle, cross
    uint32_t _t_check_last;
    uint32_t _t_update_last;
};

#endif