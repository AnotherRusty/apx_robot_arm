#ifndef SERVO_JOINT_H_
#define SERVO_JOINT_H_


#include <Arduino.h>
#include "IJoint.h"
#include "Servo.h"
#include "Config.h"

class ServoJoint: public IJoint
{
public:
    ServoJoint(){};
    ServoJoint(uint8_t pin, unsigned short max_angle);
    ~ServoJoint(){};
    void init();
    void move(unsigned short angle);
    void direct_move(unsigned short angle);
    unsigned short get_position();

    void set_reverse(bool reverse);

private:
    uint8_t _pin;
    unsigned short _angle;
    unsigned short _max_angle;
    Servo   _servo;
    uint16_t _move_interval;
    uint32_t _t_last;

    bool _reverse_flag;
};


#endif