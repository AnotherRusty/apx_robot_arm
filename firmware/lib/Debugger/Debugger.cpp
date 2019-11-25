/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#include "Debugger.h"
#include "Config.h"


Debugger::Debugger(){
#if DEBUG_ENABLE    // UNO不支持串口调试 DEBUG_ENABLE=0
    DEBUG_SERIAL.begin(DEBUG_BAUDRATE); // 初始化debug串口
#endif
}

int Debugger::printf(char *fmt, ...){
    int n = 0;
#if DEBUG_ON    // DEBUG开关宏
    va_list args;
    va_start(args, fmt);
    n = vsprintf(sprint_buf, fmt, args);
    va_end(args);
    DEBUG_SERIAL.write(sprint_buf, n);
    DEBUG_SERIAL.write("\n");
#endif
    return n;
}