#include "StepperJoint.h"
#include "Debugger.h"
#include "Config.h"

#define STEP_INTERVAL   8000 
#define CW      true
#define CCW     false

#define STEP_ANGLE 1.8
#define SUBDIVISION 8
#define PULSE_WIDTH 10

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

    _reset();
    _t_last = 0;
}

void StepperJoint::move(unsigned short angle)
{
    unsigned short target = min(max(0, angle), _max_angle);

    if((micros()-_t_last)>STEP_INTERVAL){
        if(_angle == target)
        {
            _t_last = micros();
            return;
        }
        if(target > _angle){
            if(!_reverse_flag)
                _step(CW);
            else
                _step(CCW);
            _nstep++;
        }
        else if(target < _angle){
            if(!_reverse_flag)
                _step(CCW);
            else
                _step(CW);
            _nstep--;
        }

        _t_last = micros();
    }
}

unsigned short StepperJoint::get_position(){
    _angle = _nstep / (float)SUBDIVISION * STEP_ANGLE; 
    return min(max(0, _angle), _max_angle);
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

void StepperJoint::_reset(uint32_t timeout){
    uint32_t t = millis();
    while((millis()-t)<timeout){
        if(digitalRead(_lock_pin)) // active LOW
        {
            if(!_reverse_flag)
                _step(CCW);
            else
                _step(CW);
            delayMicroseconds(2000);
        }
        else    // reach limit
        {
            _nstep = 0;
            _angle = 0;
            return;
        }
    }
}
