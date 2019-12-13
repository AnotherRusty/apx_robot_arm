#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#include <Arduino.h>
#include "Config.h"

enum EndEffectorState{
    IDLE = 0,
    GRIP = 1,
    RELEASE = 2,
};

class RobotData{
public:
    static RobotData* get(){
        static RobotData d;
        return &d;
    }

    unsigned short target_joint_angles[NUM_JOINTS];
    unsigned short current_joint_angles[NUM_JOINTS];
    uint8_t end_effector_state;
    
private:
    RobotData(){
        memset(target_joint_angles, 0, sizeof(target_joint_angles));
        memset(current_joint_angles, 0, sizeof(current_joint_angles));
        
        end_effector_state = IDLE;
    };  
};


#endif
