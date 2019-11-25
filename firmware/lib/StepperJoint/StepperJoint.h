
#ifndef STEPPER_JOINT_H_
#define STEPPER_JOINT_H_


#include <Arduino.h>
#include "IJoint.h"


class StepperJoint: public IJoint
{
public:
    StepperJoint(uint8_t enable, uint8_t dir, uint8_t step, uint8_t lock);
    ~StepperJoint(){};
    void init();
    void move(unsigned short angle);
    void direct_move(unsigned short angle){};   //not used
    unsigned short get_position();

private:
    void _step(bool direction);

    uint8_t _enable_pin, _dir_pin, _step_pin, _lock_pin;
    unsigned short _angle;
    uint16_t _nstep;
    uint32_t _t_last;
};


#endif