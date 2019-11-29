#ifndef SERVO_JOINT_H_
#define SERVO_JOINT_H_


#include <Arduino.h>
#include "IJoint.h"
#include "Servo.h"

class ServoJoint: public IJoint
{
public:
    ServoJoint(uint8_t pin, unsigned short max_angle);
    ~ServoJoint(){};
    void init();
    void move(unsigned short angle);
    void direct_move(unsigned short angle);
    unsigned short get_position();

private:
    uint8_t _pin;
    unsigned short _angle;
    unsigned short _max_angle;
    Servo   _servo;
    uint16_t _move_interval;
    uint32_t _t_last;
};


#endif