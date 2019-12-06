#include "VacuumEndEffector.h"
#include <Arduino.h>

#define MOTOR_ON    1
#define MOTOR_OFF   0
#define RELAY_ON    1
#define RELAY_OFF   0

VacuumEndEffector::VacuumEndEffector(uint8_t relay_pin){
    _has_motor_control = false;
    _motor_pin = NULL;
    _relay_pin = relay_pin;
}

VacuumEndEffector::VacuumEndEffector(uint8_t motor_pin, uint8_t relay_pin){
    _has_motor_control = true;
    _motor_pin = motor_pin;
    _relay_pin = relay_pin;
}

void VacuumEndEffector::init(){
    if(_has_motor_control){
        pinMode(_motor_pin, OUTPUT);
        digitalWrite(_motor_pin, MOTOR_OFF);
    }
    pinMode(_relay_pin, OUTPUT);
    digitalWrite(_relay_pin, RELAY_OFF);
}

void VacuumEndEffector::activate(){
    if(_has_motor_control)
        digitalWrite(_motor_pin, MOTOR_ON);
}

void VacuumEndEffector::grip(){
    digitalWrite(_relay_pin, RELAY_ON);
}

void VacuumEndEffector::release(){
    digitalWrite(_relay_pin, RELAY_OFF);
}