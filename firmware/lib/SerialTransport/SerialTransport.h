/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#ifndef SERIAL_TRANSPORT_H_
#define SERIAL_TRANSPORT_H_

#include <Arduino.h>
#include "ITransport.h"
#include "Config.h"


/* SerialTransport类
继承了ITransport接口，负责串口数据的收发
*/
class SerialTransport : public ITransport
{
public:
    SerialTransport(HardwareSerial& id, uint32_t baudrate);
    ~SerialTransport(){};
    void init();
    bool read(unsigned char& ch);
    void write(unsigned char* buf, uint8_t len);

private:
    uint32_t _baudrate;
    HardwareSerial* _serial;
};


#endif