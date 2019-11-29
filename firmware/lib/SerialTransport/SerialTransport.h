#ifndef SERIAL_TRANSPORT_H_
#define SERIAL_TRANSPORT_H_

#include <Arduino.h>
#include "ITransport.h"
#include "Config.h"


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