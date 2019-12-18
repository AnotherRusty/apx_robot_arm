#ifndef STEPPER_JOINT_H_
#define STEPPER_JOINT_H_


#include <Arduino.h>
#include "IJoint.h"
#include "Config.h"


class StepperJoint: public IJoint
{
public:
    StepperJoint(){};
    StepperJoint(uint8_t enable, uint8_t dir, uint8_t step, uint8_t lock, unsigned short _max_angle);
    ~StepperJoint(){};
    void init();
    void move(unsigned short angle);
    void direct_move(unsigned short angle){};   //not used
    unsigned short get_position();
    void set_reverse(bool reverse);

private:
    void _step(bool direction);
    void _reset(uint32_t timeout=5000); //ms

    uint8_t _enable_pin, _dir_pin, _step_pin, _lock_pin;
    unsigned short _angle;
    unsigned short _max_angle;
    uint16_t _nstep;
    uint32_t _t_last;

    bool _reverse_flag;
};


#endif