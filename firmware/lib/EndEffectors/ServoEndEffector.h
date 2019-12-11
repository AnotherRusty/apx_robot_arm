#ifndef SERVO_END_EFFECTOR_H_
#define SERVO_END_EFFECTOR_H_

#include "IEndEffector.h"
#include "ServoJoint.h"
#include "Config.h"

class ServoEndEffector : public IEndEffector
{
public:
    ServoEndEffector(uint8_t pin, unsigned short max_angle);
    ~ServoEndEffector(){};
    void init();
    void grip();
    void release();

private:
    ServoJoint _servo_joint;
    unsigned short _current_angle;
};

#endif