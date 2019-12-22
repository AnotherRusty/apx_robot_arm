from __future__ import print_function

from transport import Transport
from protocol import *
import threading
from config import*
from messages import*
from time import time as now, sleep
import yaml

import os

class EndEffectorStateList:
    IDLE = 0
    GRIP = 1
    RELEASE = 2

class RobotStatus:
    def __init__(self, num_joints):
        self.current_joint_positions = JointPos().resize(num_joints)

class Robot:
    def __init__(self):
        self.name = None
        self.__protocol = Protocol()
        self.__shutdown = False
        self.__initialized = False
        self.__configured = False
        self.__waiting_for = None
        self.__config = None

    def configure(self, name, port, baudrate):
        self.name = name
        pkg_path = os.popen('rospack find robot_arm').read()[:-1]
        try:
            config_file = pkg_path + '/scripts/config/' + self.name + '.yaml'
            with open(config_file) as f:
                conf = yaml.safe_load(f)
                f.close()
            self.__config = conf
            self.__port = port
            self.__baudrate = baudrate
            self.__num_joints = len(conf['Joints'])
        except:
            print("Error occurred during configuration. \n *** Make sure " + config_file + " exists.")
            return False
        self.__configured = True
        return True

    def initialize(self):
        if not self.__configured:
            print('robot has not been configured.')
            return False
        try:
            self.__transport = Transport(self.__port, self.__baudrate)
        except:
            print("Serial initialization failed. \n *** Check serial settings.")
            return False
        self.__proc = threading.Thread(name='robot process', target=self.__run)
        self.__proc.setDaemon(True)
        self.robot_status = RobotStatus(self.__num_joints)
        self.__initialized = True
        return True

    def start(self):
        if not self.__initialized:
            print("Error: Robot has not been initialized.")
            return False
        self.__proc.start()
        return True

    def shutdown(self):
        self.__shutdown = True  # set shutdown flag

    def get_joint_angles(self):
        self.__set_wait(MsgId.RET_JOINT_POSITIONS)
        self.__transport.write(self.__protocol.encode(MsgId.GET_JOINT_POSITIONS, Empty()))
        if self.__wait():
            return self.robot_status.current_joint_positions.angles
        else:
            return False

    def set_joint_angles(self, angles):
        print("setting joint angles at ", angles, "\r\n")
        msg = JointPos().resize(self.__num_joints)
        for i in range(self.__num_joints):
            msg.angles[i] = angles[i]
        self.__transport.write(self.__protocol.encode(MsgId.SET_JOINT_POSITIONS, msg))
    
    def set_end_effector_state(self, state):
        if state == EndEffectorStateList.IDLE:
            return
        elif state == EndEffectorStateList.GRIP:
            print("End_effector ----> GRIP \r\n")
        elif state == EndEffectorStateList.RELEASE:
            print("End_effector ----> RELEASE \r\n")
        msg = EndEffectorState()
        msg.state = state
        self.__transport.write(self.__protocol.encode(MsgId.SET_END_EFFECTOR_STATE, msg))        

    def robot_config(self):
        return self.__config

    def __run(self):
        while not self.__shutdown:
            c = self.__transport.read()
            # print(c)
            if self.__protocol.parse(c):
                self.__process_message(self.__protocol.get_message()) 

    def __process_message(self, msg_id):
        if msg_id == MsgId.MOVE_DONE:
            pass
        elif msg_id == MsgId.RET_JOINT_POSITIONS:
            self.__protocol.decode(msg_id, self.robot_status.current_joint_positions)
            if self.__waiting_for == MsgId.RET_JOINT_POSITIONS:
                self.__waiting_for = None

    def __set_wait(self, msg_id):
        self.__waiting_for = msg_id
    
    def __wait(self, timeout=0.5):
        _timeout = now()+timeout
        while now() < _timeout:
            if self.__waiting_for is None:
                return True
        return False
            