#include "StepperJoint.h"
#include "Debugger.h"
#include "Config.h"

#define STEP_INTERVAL   10000 
#define CW      true
#define CCW     false

#define STEP_ANGLE 1.8
#define SUBDIVISION 1
#define PULSE_WIDTH 50

StepperJoint::StepperJoint(uint8_t enable_pin, uint8_t dir_pin, uint8_t step_pin, uint8_t lock_pin, unsigned short max_angle){
    _enable_pin = enable_pin;
    _dir_pin = dir_pin;
    _step_pin = step_pin;
    _lock_pin = lock_pin;
    _max_angle = max_angle;

    _reverse_flag = false;
}

void StepperJoint::init()
{
    pinMode(_enable_pin, OUTPUT); 
    pinMode(_dir_pin, OUTPUT);    
    pinMode(_step_pin, OUTPUT);    
    pinMode(_lock_pin, INPUT);    

    digitalWrite(_enable_pin, LOW);

    if(!_reset())
        return;
    _t_last = 0;
}

void StepperJoint::move(unsigned short angle){
    if(!digitalRead(_lock_pin))
        return;

    unsigned short target;
    if(!_reverse_flag)
        target = angle;
    else
        target = _max_angle - angle;

    uint16_t nstep = (float)target / STEP_ANGLE * (float)SUBDIVISION;
    if((micros()-_t_last)>STEP_INTERVAL){
        if(nstep == _nstep)
            return;
        if(nstep > _nstep){
            _step(CW);
            _nstep++;
        }
        if(nstep < _nstep){
            _step(CCW);
            _nstep--;
        }

        _t_last = micros();
    }
}

unsigned short StepperJoint::get_position(){
    if(!_reverse_flag)
        _angle = _nstep / (float)SUBDIVISION * STEP_ANGLE; 
    else
        _angle = _max_angle - _nstep / (float)SUBDIVISION * STEP_ANGLE; 
    return _angle;
}

void StepperJoint::set_reverse(bool reverse){
    _reverse_flag = reverse;
}

void StepperJoint::_step(bool direction)
{   
    digitalWrite(_dir_pin, direction);    
    digitalWrite(_step_pin, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(_step_pin, LOW);
    delayMicroseconds(PULSE_WIDTH);    
}   

bool StepperJoint::_reset(uint16_t timeout){
    uint16_t t = millis();
    while((millis()-t)<timeout){
        if(digitalRead(_lock_pin)) // active LOW
            if(!_reverse_flag)
                _step(CCW);
            else
                _step(CW);
            delayMicroseconds(STEP_INTERVAL);
    }
    _nstep = 0;
    _angle = 0;
}
