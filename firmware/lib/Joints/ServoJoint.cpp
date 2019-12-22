#include "ServoJoint.h"
#include "Debugger.h"
#include "Config.h"


#define INTERVAL 10000

ServoJoint::ServoJoint(uint8_t pin, unsigned short max_angle){
    _pin = pin;
    _max_angle = max_angle;
    _move_interval = double(INTERVAL) / SERVO_SPEED;
    _reverse_flag = false;
}

void ServoJoint::init(){
    Debugger::get()->printf("Servo on pin %d", _pin);
    _servo.attach(_pin);
    delayMicroseconds(50);
    _servo.write(90);
    _angle = 90;
}

void ServoJoint::move(unsigned short angle){
    Debugger::get()->printf("Move sevo on pin%d to angle: %d", _pin, angle);
    
    unsigned short target;
    if(!_reverse_flag)
        target = min(max(0, angle), _max_angle);
    else
        target = min(max(0, _max_angle-angle), _max_angle);

    if((micros()-_t_last)>_move_interval){
        if(target ==_angle)
            return;
        if(target >_angle)
        {
            _servo.write(++_angle);
        }
        else if (target <_angle)
        {
            _servo.write(--_angle);
        }
        _t_last = micros();
    }
}

void ServoJoint::direct_move(unsigned short angle){
    if(!_reverse_flag)
        _angle = angle;
    else
        _angle = _max_angle - angle;
    _servo.write(_angle);
}

unsigned short ServoJoint::get_position(){
    if(!_reverse_flag)
        return _angle;
    else
        return (_max_angle-_angle);
}

void ServoJoint::set_reverse(bool reverse){
    _reverse_flag = reverse;
}