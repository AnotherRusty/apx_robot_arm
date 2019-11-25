/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#include "Robot.h"
#include "RobotData.h"
#include "Debugger.h"


// 构造函数
// 参数： None
Robot::Robot(){
    _num_joints = 0;    //未添加任何关节
}

// 初始化
// 参数： None
// 返回值： bool  成功/失败
bool Robot::init(){
    Debugger::get()->printf("Initiating %s ... ", ROBOT_NAME);
    if (_num_joints!=NUM_JOINTS){
        Debugger::get()->printf("Should have %d joints, but only %d configured.", NUM_JOINTS, _num_joints);
        return false;
    } 

    // 初始化各个关节
    Debugger::get()->printf("Initiating joints ...");
    for(int i=0; i<NUM_JOINTS; i++){
        if(_joints[i]){
            _joints[i]->init();
        }
    }

    // 重置关节
    Debugger::get()->printf("Reset joint to initial positions.");
    _reset_joints();

    Debugger::get()->printf("Robot init done.");
    return true;    
}

// 主函数，被程序主循环loop()调用
// 参数： None
// 返回值： None
void Robot::run(){
    // 从RobotData中获取目标关节角度和当前关节角度，针对每个关节对比如果不同则调用相应关节的move()函数
    // 之后获取该关节的当前位置并更新到RobotData
    RobotData* rd = RobotData::get();
    for(int i=0; i<NUM_JOINTS; i++){
        if(rd->current_joint_angles[i]!=rd->target_joint_angles[i]){
            _joints[i]->move(rd->target_joint_angles[i]);
            rd->current_joint_angles[i] = _joints[i]->get_position();   // update current joint position
        }
    }
}

// add_joint，添加关节
// 参数： id     关节编号
//       j      IJoint对象
// 返回值： None
void Robot::add_joint(uint8_t id, IJoint* j){
    Debugger::get()->printf("Joint %d added to robot.", id);
    if (id<NUM_JOINTS){
        if(!_joints[id]){
            _joints[id] = j;
            _num_joints++;
        }
    }
}

// get_joint，获取关节
// 参数： id     关节编号
// 返回值： IJoint对象指针
IJoint* Robot::get_joint(uint8_t id){
    if (id>=NUM_JOINTS) return NULL;
    return _joints[id];
}

// _reset_joints，重置关节
// 参数： None
// 返回值： None
void Robot::_reset_joints(){
    //所有关节默认初始位置为90度
    Debugger::get()->printf("Reset joint positions. ");
    RobotData* rd = RobotData::get();
    for(int i=0; i<NUM_JOINTS; i++){
        rd->target_joint_angles[i] = 90;
    }
}
