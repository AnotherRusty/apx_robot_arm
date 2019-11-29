#include "Debugger.h"
#include "Config.h"


Debugger::Debugger(){
#if DEBUG_ENABLE
    DEBUG_SERIAL.begin(DEBUG_BAUDRATE);
#endif
}

int Debugger::printf(char *fmt, ...){
    int n = 0;
#if DEBUG_ON
    va_list args;
    va_start(args, fmt);
    n = vsprintf(sprint_buf, fmt, args);
    va_end(args);
    DEBUG_SERIAL.write(sprint_buf, n);
    DEBUG_SERIAL.write("\n");
#endif
    return n;
}