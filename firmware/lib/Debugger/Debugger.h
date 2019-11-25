/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#ifndef DEBUGGER_H_
#define DEBUGGER_H_

#include <Arduino.h>
#include "Config.h"

/* Debugger类
负责串口调试格式化输出
作为单例模式，静态对象可以通过Debugger::get()函数获取
在其他文件中只需添加头文件后，直接使用Debugger::printf()进行输出
*/
class Debugger
{
public:
    static Debugger* get(){
        // 静态成员函数，返回intance实例
        static Debugger instance;
        return &instance;
    }

    int printf(char *fmt, ...); //格式化输出

private:
    Debugger(); // 构造函数私有，不允许通过类名创建对象
    char sprint_buf[1024];  //buffer
};

#endif