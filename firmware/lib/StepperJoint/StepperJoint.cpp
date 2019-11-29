#include "StepperJoint.h"
#include "Debugger.h"
#include "Config.h"

#define MOVE_INTERVAL   10000 
#define CW      true
#define CCW     false

#define NORMAL_POSITION 0
#define STEP_ANGLE 1.8
#define SUBDIVISION 1
#define PULSE_WIDTH 50

StepperJoint::StepperJoint(uint8_t enable_pin, uint8_t dir_pin, uint8_t step_pin, uint8_t lock_pin){
    _enable_pin = enable_pin;
    _dir_pin = dir_pin;
    _step_pin = step_pin;
    _lock_pin = lock_pin;
}

void StepperJoint::init()
{
    pinMode(_enable_pin, OUTPUT); 
    pinMode(_dir_pin, OUTPUT);    
    pinMode(_step_pin, OUTPUT);    
    pinMode(_lock_pin, INPUT);    

    digitalWrite(_enable_pin, LOW);

    _angle = NORMAL_POSITION;

    _t_last = 0;
}

void StepperJoint::move(unsigned short angle){
    if(!digitalRead(_lock_pin))
        return;
    uint16_t nstep = (float)angle / STEP_ANGLE * (float)SUBDIVISION;
    if((micros()-_t_last)>MOVE_INTERVAL){
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
    _angle = _nstep / (float)SUBDIVISION * STEP_ANGLE; 
    return _angle;
}

void StepperJoint::_step(bool direction)
{   
    digitalWrite(_dir_pin, direction);    
    digitalWrite(_step_pin, HIGH);
    delayMicroseconds(PULSE_WIDTH);
    digitalWrite(_step_pin, LOW);
    delayMicroseconds(PULSE_WIDTH);    
}   
