#!/usr/bin/env python
#coding=utf-8

from __future__ import print_function

import rospy
import sys, select, termios, tty
from lib.robot import Robot, EndEffectorStateList
from time import time as now, sleep
import threading
import operator

UPDATE_INTERVAL = 0.5 # seconds
ROBOT_INIT_TIME = 3.0   # second

NUM_JOINTS = 3

# joint id 
LR = 0
UD1 = 1
UD2 = 2

msg = """
Use keyboard to control the robot arm.
---------------------------
Move arm:
    w: up
    s: down
    a: left
    r: right
---------------------------
Select certain joint:
    u: joint 1
    i: joint 2
---------------------------
Move end effector:
    j: grip
    k: release
---------------------------
Exit with Ctrl-C
"""

def getKey():
    tty.setraw(sys.stdin.fileno())
    select.select([sys.stdin], [], [], 0)
    key = sys.stdin.read(1)
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key

def shutdown():
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    sys.exit()

class KeyboardController():
    def __init__(self):
        self.robot = Robot()
        self.current_angles = [0 for i in range(NUM_JOINTS)]
        self.target_angles = [0 for i in range(NUM_JOINTS)]
        self.target_end_effector_state = EndEffectorStateList.IDLE
        self.selected = UD2 # default select JOINT2
        self.shutdown = False

    def setup(self):
        self.__load_params()
        self.__setup_robot()

    def start(self):
        print(msg)

        feedback_thread = threading.Thread(name='feedback', target=self.__feedback)
        feedback_thread.setDaemon(True)
        feedback_thread.start()

        listen_key_thread = threading.Thread(name='listen_key', target=self.__listen_key)
        listen_key_thread.setDaemon(True)
        listen_key_thread.start()

        try:
            while not self.shutdown:
                if not operator.eq(self.current_angles, self.target_angles):
                    self.robot.set_joint_angles(self.target_angles)
                
                if self.target_end_effector_state != EndEffectorStateList.IDLE:
                    self.robot.set_end_effector_state(self.target_end_effector_state)
                
                self.target_end_effector_state = EndEffectorStateList.IDLE
                self.cmd = '' # clear command

        except Exception as e:
            print(e)

        finally:
            shutdown()

    def __load_params(self):
        # get parameters from server
        self.param_arm_model = rospy.get_param('~arm_model', 'unspecified')
        self.param_port = rospy.get_param('~port', '/dev/ttyUSB0')
        self.param_baudrate = rospy.get_param('~baudrate', 9600)

        if self.param_arm_model == 'unspecified':
            print('*** Error: Robot arm model not specified.')
            quit()

    def __setup_robot(self):
        # initialize robot
        if not self.robot.configure(name=self.param_arm_model, port=self.param_port, baudrate=self.param_baudrate):
            shutdown()

        if not self.robot.initialize():
            shutdown()

        if not self.robot.start():
            shutdown()
        
        print("Wait for robot to reset.")
        sleep(ROBOT_INIT_TIME)

    def __feedback(self):
        t = now()
        while(1):
            if (now()- t)>UPDATE_INTERVAL:
                current_angles = self.robot.get_joint_angles()
                if current_angles:
                    for i in range(NUM_JOINTS):
                        self.current_angles[i] = current_angles[i]
                t = now()

    def __listen_key(self):
        try:
            while(1):
                key = getKey()
                if key == '\x03':
                    break
                
                if key in 'wasd':
                    if key == 'a':      # move left
                        if self.target_angles[LR] > self.robot.robot_config()['Joints'][LR]['params']['min_angle']:
                            self.target_angles[LR] -= 1
                    elif key == 'd':    # move right
                        if self.target_angles[LR] < self.robot.robot_config()['Joints'][LR]['params']['max_angle']:
                            self.target_angles[LR] += 1
                    elif key == 'w':    # move up
                        if self.target_angles[self.selected] > self.robot.robot_config()['Joints'][self.selected]['params']['min_angle']:
                            self.target_angles[self.selected] -= 1
                    elif key == 's':    # move down
                        if self.target_angles[self.selected] < self.robot.robot_config()['Joints'][self.selected]['params']['max_angle']:
                            self.target_angles[self.selected] += 1
                elif key == 'u':
                    self.selected = UD1
                    print('**** JOINT 1 selected.')
                elif key == 'i':
                    self.selected = UD2
                    print('**** JOINT 2 selected.')
                elif key == 'j':
                    self.target_end_effector_state = EndEffectorStateList.GRIP
                elif key == 'k':
                    self.target_end_effector_state = EndEffectorStateList.RELEASE
                else:
                    continue

        except Exception as e:
            print(e)
        
        finally:
            self.shutdown = True


if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)

    rospy.init_node('arm_bringup', anonymous=False)
    
    controller = KeyboardController()
    controller.setup()
    controller.start()




    

