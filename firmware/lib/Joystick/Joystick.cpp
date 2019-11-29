#include "Joystick.h"
#include <stdio.h>
#include "RobotData.h"
#include "Config.h"
#include "Debugger.h"


#define LEFT      0
#define RIGHT     1
#define UP        2
#define DOWN      3
#define SQUARE    4
#define TRIANGLE  5
#define CIRCLE    6
#define CROSS     7

#define COMMAND_UPDATE_INTERVAL 10000 // us
#define CHECK_INTERVAL 2000 //us
#define INCREMENT   1 // degree


Joystick::Joystick(){
  _type = 0;
  memset(_buttons, 0,sizeof(_buttons)); // clear buttons
  _t_check_last = micros();
  _t_update_last = micros();
}

bool Joystick::init(){
  Debugger::get()->printf("Init joystick ...");
  _error = _ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, PRESSURES, RUMBLE);
  _type = _ps2x.readType();

  if ((_error == 0)&&(_type != 2)) 
    return true;
  else 
    return false;
}

void Joystick::check_command(){
  if((micros()-_t_check_last)>CHECK_INTERVAL){
    _check();

    if((micros()-_t_update_last)>COMMAND_UPDATE_INTERVAL){
      unsigned short current_target; 
      unsigned short new_target;
      RobotData* rd = RobotData::get();

      // left/right to move join0
      if((_buttons[LEFT]) && (!_buttons[RIGHT]))
      {
        current_target = rd->target_joint_angles[0];
        new_target = min(max(0, current_target+INCREMENT), 180);
        rd->target_joint_angles[0] = new_target;
        return;
      }
      if((_buttons[RIGHT]) && (!_buttons[LEFT]))
      {
        current_target = rd->target_joint_angles[0];
        new_target = min(max(0, current_target-INCREMENT), 180);
        rd->target_joint_angles[0] = new_target;
        return;
      }

      // up/down
      if(_buttons[UP])
      { 
        // with joint selected
        if(_buttons[SQUARE])
        {
          current_target = rd->target_joint_angles[1];
          new_target = min(max(0, current_target-INCREMENT), 180);
          rd->target_joint_angles[1] = new_target;
          return;
        }
        if(_buttons[TRIANGLE])
        {
          current_target = rd->target_joint_angles[2];
          new_target = min(max(0, current_target-INCREMENT), 180);
          rd->target_joint_angles[2] = new_target;
          return;
        }
        if(_buttons[CIRCLE])
        {
          current_target = rd->target_joint_angles[3];
          new_target = min(max(0, current_target-INCREMENT), 180);
          rd->target_joint_angles[3] = new_target;
          return;        
        }
        if(_buttons[CROSS])
        {
          current_target = rd->target_joint_angles[4];
          new_target = min(max(0, current_target-INCREMENT), 180);
          rd->target_joint_angles[4] = new_target;
          return;
        }

        // no joint selected
        // todo
      }

      if(_buttons[DOWN])
      {
        if(_buttons[SQUARE])
        {
          current_target = rd->target_joint_angles[1];
          new_target = min(max(0, current_target+INCREMENT), 180);
          rd->target_joint_angles[1] = new_target;
          return;
        }
        if(_buttons[TRIANGLE])
        {
          current_target = rd->target_joint_angles[2];
          new_target = min(max(0, current_target+INCREMENT), 180);
          rd->target_joint_angles[2] = new_target;
          return;
        }
        if(_buttons[CIRCLE])
        {
          current_target = rd->target_joint_angles[3];
          new_target = min(max(0, current_target+INCREMENT), 180);
          rd->target_joint_angles[3] = new_target;
          return;        
        }
        if(_buttons[CROSS])
        {
          current_target = rd->target_joint_angles[4];
          new_target = min(max(0, current_target+INCREMENT), 180);
          rd->target_joint_angles[4] = new_target;
          return;
        }

        // no joint selected
        // todo
      }

      _t_update_last = micros();
    }
  
  _t_check_last = micros();
  }
}

void Joystick::_check(){
  memset(_buttons, 0,sizeof(_buttons)); // clear buttons
  _ps2x.read_gamepad();

  if (_ps2x.Button(PSB_PAD_LEFT))
    _buttons[LEFT] = 1;
  if (_ps2x.Button(PSB_PAD_RIGHT))
    _buttons[RIGHT] = 1;
  if (_ps2x.Button(PSB_PAD_UP))
    _buttons[UP] = 1;
  if (_ps2x.Button(PSB_PAD_DOWN))
    _buttons[DOWN] = 1;
  if (_ps2x.Button(PSB_SQUARE))
    _buttons[SQUARE] = 1;
  if (_ps2x.Button(PSB_TRIANGLE))
    _buttons[TRIANGLE] = 1;
  if (_ps2x.Button(PSB_CIRCLE))
    _buttons[CIRCLE] = 1;
  if (_ps2x.Button(PSB_CROSS))
    _buttons[CROSS] = 1;

  // Debugger::get()->printf("PS2: [%d, %d, %d, %d, %d, %d, %d, %d" 
  //     ,_buttons[0], _buttons[1], _buttons[2], _buttons[3]
  //     ,_buttons[4], _buttons[5], _buttons[6], _buttons[7]);
}
