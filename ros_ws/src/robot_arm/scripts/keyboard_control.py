#!/usr/bin/env python

from __future__ import print_function

import roslib
import rospy

import sys, select, termios, tty

msg = """
Use keyboard to control the robot arm.
---------------------------
MOve arm:
    w: up
    s: down
    a: left
    r: right
---------------------------
Move end effector:
    j: grip
    k: release
---------------------------
Exit with Ctrl+C
"""

class KeyEvents:
    up = 'w'
    down = 's'
    left = 'a'
    right = 'd'
    grip = 'j'
    release = 'k'


def getKey():
    tty.setraw(sys.stdin.fileno())
    select.select([sys.stdin], [], [], 0)
    key = sys.stdin.read(1)
    termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)
    return key


def vels(speed,turn):
    return "currently:\tspeed %s\tturn %s " % (speed,turn)

if __name__=="__main__":
    settings = termios.tcgetattr(sys.stdin)

    # pub = rospy.Publisher('arm_control', Twist, queue_size = 1)
    rospy.init_node('keyboard_control')

    try:
        print(msg)
        while(1):
            key = getKey()
            if key == KeyEvents.up:
                print('up')
            if key == KeyEvents.down:
                print('down')
            if key == KeyEvents.left:
                print('left')
            if key == KeyEvents.right:
                print('right')
            if key == KeyEvents.grip:
                print('grip')
            if key == KeyEvents.release:
                print('release')
            if (key == '\x03'):
                break

    except Exception as e:
        print(e)

    finally:
        termios.tcsetattr(sys.stdin, termios.TCSADRAIN, settings)