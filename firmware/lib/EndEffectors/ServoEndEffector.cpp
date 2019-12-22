#include "ServoEndEffector.h"

#define STEP 1

ServoEndEffector::ServoEndEffector(uint8_t pin, unsigned short max_angle){
    _servo_joint = ServoJoint(pin, max_angle);
}

void ServoEndEffector::init(){
    _servo_joint.init();
    _current_angle = _servo_joint.get_position();
}

void ServoEndEffector::grip(){
    _servo_joint.move(_current_angle - STEP);
    _current_angle = _servo_joint.get_position();
}

void ServoEndEffector::release(){
    _servo_joint.move(_current_angle + STEP);
    _current_angle = _servo_joint.get_position();
}