#ifndef ROBOT_H_
#define ROBOT_H_

#include "IJoint.h"
#include "Config.h"
#include "IEndEffector.h"

class Robot
{
public:
    Robot();
    ~Robot(){};
    bool init();
    void run();
    void add_joint(uint8_t id, IJoint* j);
    void add_end_effector(IEndEffector* e);
    IJoint* get_joint(uint8_t id);
    IEndEffector* get_end_effector();

private:
    void _reset();

    IJoint* _joints[NUM_JOINTS] = {0};
    uint8_t _num_joints;
    IEndEffector* _end_effector = NULL;

};


#endif