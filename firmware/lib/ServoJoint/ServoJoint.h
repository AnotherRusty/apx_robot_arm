/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#ifndef SERVO_JOINT_H_
#define SERVO_JOINT_H_


#include <Arduino.h>
#include "IJoint.h"
#include "Servo.h"

/* ServoJoint类
舵机关节，集成了IJoint，负责具体驱动舵机动作
利用了第三方Arduino库 Servo 控制舵机

*/
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