#include "ServoEndEffector.h"


ServoEndEffector::ServoEndEffector(uint8_t pin, unsigned short max_angle){
    _servo_joint = &ServoJoint(pin, max_angle);
}

void ServoEndEffector::init(){
    _servo_joint->ini
}

void ServoEndEffector::grip(){

}

void ServoEndEffector::release(){

}