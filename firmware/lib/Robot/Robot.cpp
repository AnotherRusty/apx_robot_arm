#include "Robot.h"
#include "RobotData.h"
#include "Debugger.h"

Robot::Robot(){
    _num_joints = 0;
}

bool Robot::init(){
    Debugger::get()->printf("Initializing %s ... ", ROBOT_NAME);
    if (_num_joints!=NUM_JOINTS){
        Debugger::get()->printf("Should have %d joints, but only %d configured.", NUM_JOINTS, _num_joints);
        return false;
    } 

    Debugger::get()->printf("Initializing joints ...");
    for(int i=0; i<NUM_JOINTS; i++){
        if(_joints[i]){
            _joints[i]->init();
        }
    }

    Debugger::get()->printf("Initializing end effector ...");
    _end_effector->init();

    Debugger::get()->printf("Reset robot.");
    _reset();

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

void Robot::add_end_effector(IEndEffector* e){
    Debugger::get()->printf("End effector added to robot.");
    if(!_end_effector)
        _end_effector = e;
}

IJoint* Robot::get_joint(uint8_t id){
    if (id>=NUM_JOINTS) return NULL;
    return _joints[id];
}

IEndEffector* Robot::get_end_effector(){
    return _end_effector;
}

void Robot::_reset(){
    Debugger::get()->printf("Reset robot ... ");
    RobotData* rd = RobotData::get();
    for(int i=0; i<NUM_JOINTS; i++){
        rd->target_joint_angles[i] = 90;
    }

    // todo
}
