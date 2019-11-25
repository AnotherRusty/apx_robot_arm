/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#ifndef ROBOT_DATA_H_
#define ROBOT_DATA_H_

#include <Arduino.h>
#include "Config.h"

/* RobotData类
单例类，静态实例保存了目标关节角度和当前关节角度，通过RobotData::get()获取
*/
class RobotData{
public:
    static RobotData* get(){
        static RobotData d;
        return &d;
    }

    unsigned short target_joint_angles[NUM_JOINTS];
    unsigned short current_joint_angles[NUM_JOINTS];

private:
    //构造函数私有，不允许通过类名创建RobotData对象
    RobotData(){
        memset(target_joint_angles, 0, sizeof(target_joint_angles));
        memset(current_joint_angles, 0, sizeof(current_joint_angles));
    };  
};


#endif
