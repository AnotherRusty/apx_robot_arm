#ifndef VACUUM_END_EFFECTOR_H_
#define VACUUM_END_EFFECTOR_H_

#include "IEndEffector.h"
#include "Config.h"


class VacuumEndEffector : public IEndEffector
{
public:
    VacuumEndEffector(uint8_t relay_pin);
    VacuumEndEffector(uint8_t motor_pin, uint8_t relay_pin);

    void init();
    void grip();
    void release();

    void activate();
    
private:
    bool _has_motor_control;    // flag : true if motor control pin is available
    uint8_t _motor_pin, _relay_pin;

};

#endif