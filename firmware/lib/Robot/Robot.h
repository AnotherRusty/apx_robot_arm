#ifndef ROBOT_H_
#define ROBOT_H_

#include "IJoint.h"
#include "Config.h"


class Robot
{
public:
    Robot();
    ~Robot(){};
    bool init();
    void run();
    void add_joint(uint8_t id, IJoint* j);
    IJoint* get_joint(uint8_t id);

private:
    void _reset_joints();

    IJoint* _joints[NUM_JOINTS] = {0};
    uint8_t _num_joints;

};


#endif