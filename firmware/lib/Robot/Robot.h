/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#ifndef ROBOT_H_
#define ROBOT_H_

#include "IJoint.h"
#include "Config.h"


/* Robot类
管理具体的关节，执行具体动作命令
*/
class Robot
{
public:
    Robot();
    ~Robot(){};
    bool init();
    void run();
    void add_joint(uint8_t id, IJoint* j);  //添加关节IJoint对象
    IJoint* get_joint(uint8_t id);  //获取关节IJoint对象

private:
    void _reset_joints();   //重置关节

    IJoint* _joints[NUM_JOINTS] = {0};  //关节对象数组，NUM_JOINTS为机器人总关节数
    uint8_t _num_joints;

};


#endif