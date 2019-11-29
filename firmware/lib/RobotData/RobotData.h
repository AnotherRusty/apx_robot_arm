#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#include <Arduino.h>
#include "Config.h"

class RobotData{
public:
    static RobotData* get(){
        static RobotData d;
        return &d;
    }

    unsigned short target_joint_angles[NUM_JOINTS];
    unsigned short current_joint_angles[NUM_JOINTS];

private:
    RobotData(){
        memset(target_joint_angles, 0, sizeof(target_joint_angles));
        memset(current_joint_angles, 0, sizeof(current_joint_angles));
    };  
};


#endif
