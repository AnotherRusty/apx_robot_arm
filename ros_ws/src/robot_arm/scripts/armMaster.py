#coding=utf-8

import sys
if sys.version < '3':   # python 2.x
    import Tkinter as tk
    from Tkinter import *
    import ttk
else:                   # python 3.x
    import tkinter as tk
    from tkinter import *
    from tkinter import ttk

import yaml
import os, platform, sys
from lib.robot import Robot
import threading
from time import time as now, sleep


WINDOW_SIZE_1 = "300x180" # width*height
WINDOW_SIZE_2 = "600x400" # width*height
UPDATE_INTERVAL = 0.5 # seconds
MODEL_LIST = ["R290", "victor", "neo"]


class Application(object):
    def __init__(self):
        self.root = tk.Tk()
        self.root.geometry(WINDOW_SIZE_1)
        self.root.title("ArmMaster - OPENARM control console")
        self.root.resizable(0, 0)
        sysstr = platform.system()
        if sysstr == "Windows":
            self.root.iconbitmap(bitmap='res/logo.ico')
        elif sysstr == "Linux":
            self.root.iconbitmap(bitmap='@res/logo.xbm')

        self.init_gui()

    def init_gui(self):
        self.setting_frame = Frame(self.root)
        
        self.robot = Robot()
        Label(self.setting_frame, text="类型").grid(row=0, column=0, pady=20, sticky=W)
        self.robot_model = ttk.Combobox(self.setting_frame, values=MODEL_LIST)
        self.robot_model.current(0)
        self.robot_model.grid(row=0, column=1, pady=20)

        self.port = StringVar(value="/dev/tty")
        Label(self.setting_frame, text="端口").grid(row=1, column=0, sticky=W)
        self.port_entry = Entry(self.setting_frame, textvariable=self.port)
        self.port_entry.grid(row=1, column=1, sticky=W+E)

        self.baudrate = StringVar(value="9600")
        Label(self.setting_frame, text="波特率").grid(row=2, column=0, pady=5, sticky=W)
        self.baudrate_entry = Entry(self.setting_frame, textvariable=self.baudrate)
        self.baudrate_entry.grid(row=2, column=1, pady=5, sticky=W+E)

        Button(self.setting_frame, text="连接", width=10, command=self.start_main).grid(row=3, column=1, pady=10, sticky=E)
        
        self.setting_frame.pack()

    def start_main(self):
        if self.robot_model.current() == -1:
            return

        model = self.robot_model.get()
        port = self.port.get()
        baudrate = int(self.baudrate.get())

        print("Configuring robot ...")
        if not self.robot.configure(model, port, baudrate):
            return
        
        print("Initializing robot ...")
        if not self.robot.initialize():
            return
        
        sleep(2)
        print("Loading ...")
        self.robot.start()

        config_file = 'config/'+model+'.yaml'
        with open(config_file) as f:
            conf = yaml.safe_load(f)
            f.close()

        self.joints = conf['Joints']
        self.num_joints = len(self.joints)
        self.joint_scales = []
        self.joint_angles = [IntVar() for i in range(self.num_joints)]
        self.current_angles = [IntVar() for i in range(self.num_joints)]
        self.joint_progresses = []
        self.joint_feedback = []

        self.main_frame = Frame(self.root)

    # control container
        self.control_container = Frame(self.main_frame)
        # joint scales
        for i in range(self.num_joints):
            Label(self.control_container, text=self.joints[i]["name"]).grid(row=i, column=0, padx=10, pady=10)
            sc = Scale(self.control_container, orient=HORIZONTAL, sliderlength=10, resolution=1, \
                            from_=self.joints[i]['params']['min_angle'], \
                                to=self.joints[i]['params']['max_angle'], \
                                    variable=self.joint_angles[i])
            sc.grid(row=i, column=1, pady=10)
            sc.set(self.joints[i]['params']['norm'])
            self.joint_scales.append(sc)
        
        # set button
        Button(self.control_container, text="设置", activebackground='grey', command=self.set_joints).grid(row=6, column=0, pady=20, sticky=W+E+N+S)
        Button(self.control_container, text="重置", activebackground='grey', command=self.reset_joints).grid(row=6, column=1, pady=20, sticky=W+E+N+S)
        
        self.control_container.grid(row=0, column=0, rowspan=6, columnspan=2, padx=10, sticky=W+E+N+S)

    # monitor container
        self.monitor_contrainer = Frame(self.main_frame)
        # current angles info
        Label(self.monitor_contrainer, text='当前关节角度: ').grid(row=0, column=1)
        for i in range(self.num_joints):
            pb = ttk.Progressbar(self.monitor_contrainer, orient=HORIZONTAL, maximum=180, length=300, mode='determinate', variable=self.current_angles[i])
            pb.grid(row=i+1, column=1, pady=20)
            self.joint_progresses.append(pb)
            lb = Label(self.monitor_contrainer, width=10, textvariable=self.current_angles[i])
            lb.grid(row=i+1, column=2, pady=20)
            self.joint_feedback.append(lb)

        self.monitor_contrainer.grid(row=0, column=2, rowspan=6, columnspan=3, padx=30, sticky=W+E+N+S)

        self.setting_frame.pack_forget()
        self.root.geometry(WINDOW_SIZE_2)
        self.main_frame.pack()

    # monitor thread
        self.monit_loop = threading.Thread(name='monitor', target=self.monitor)
        self.monit_loop.setDaemon(True)
        self.monit_loop.start()

    def loop(self):
        self.root.mainloop()
    
    def init_robot(self):
        self.robot.configure()
        self.robot.initialize()
        sleep(2)
        self.robot.start()

    def set_joints(self):
        angles = []
        for i in range(self.num_joints):
            angles.append(self.joint_angles[i].get())
        self.robot.set_joint_angles(angles)

    def reset_joints(self):
        angles = []
        for i in range(self.num_joints):
            angle = self.joints[i]['params']['norm']
            self.joint_angles[i].set(angle)
            angles.append(angle)
        self.robot.set_joint_angles(angles) 

    def monitor(self):
        t = now()
        while True:
            if (now()- t)>UPDATE_INTERVAL:
                current_angles = self.robot.get_joint_angles()
                if current_angles:
                    for i in range(self.num_joints):
                        self.current_angles[i].set(current_angles[i])
                    t = now()


if __name__ == "__main__":
    app = Application()
    app.loop()
