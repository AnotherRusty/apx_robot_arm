#include "Robot.h"
#include "RobotData.h"
#include "Debugger.h"

Robot::Robot(){
    _num_joints = 0;
}

bool Robot::init(){
    Debugger::get()->printf("Initiating %s ... ", ROBOT_NAME);
    if (_num_joints!=NUM_JOINTS){
        Debugger::get()->printf("Should have %d joints, but only %d configured.", NUM_JOINTS, _num_joints);
        return false;
    } 

    Debugger::get()->printf("Initiating joints ...");
    for(int i=0; i<NUM_JOINTS; i++){
        if(_joints[i]){
            _joints[i]->init();
        }
    }

    Debugger::get()->printf("Reset joint to initial positions.");
    _reset_joints();

    Debugger::get()->printf("Robot init done.");
    return true;    
}

void Robot::run(){
    RobotData* rd = RobotData::get();
    for(int i=0; i<NUM_JOINTS; i++){
        if(rd->current_joint_angles[i]!=rd->target_joint_angles[i]){
            _joints[i]->move(rd->target_joint_angles[i]);
            rd->current_joint_angles[i] = _joints[i]->get_position();
        }
    }
}

void Robot::add_joint(uint8_t id, IJoint* j){
    Debugger::get()->printf("Joint %d added to robot.", id);
    if (id<NUM_JOINTS){
        if(!_joints[id]){
            _joints[id] = j;
            _num_joints++;
        }
    }
}

IJoint* Robot::get_joint(uint8_t id){
    if (id>=NUM_JOINTS) return NULL;
    return _joints[id];
}

void Robot::_reset_joints(){
    Debugger::get()->printf("Reset joint positions. ");
    RobotData* rd = RobotData::get();
    for(int i=0; i<NUM_JOINTS; i++){
        rd->target_joint_angles[i] = 90;
    }
}
