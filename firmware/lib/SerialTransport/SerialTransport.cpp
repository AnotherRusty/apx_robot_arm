#include "SerialTransport.h"
#include <Arduino.h>
#include "Debugger.h"


void serialEvent(){

}

SerialTransport::SerialTransport(HardwareSerial& ser, uint32_t baudrate){
    _serial = &ser;
    _baudrate = baudrate;
}

void SerialTransport::init(){
    Debugger::get()->printf("Serial transport begin at %ld hz.", _baudrate);
    _serial->begin(_baudrate);
}

bool SerialTransport::read(unsigned char& ch){
    if (_serial->available()){
        ch = _serial->read();
        return true;
    }
    return false;
}

void SerialTransport::write(unsigned char* buf, unsigned char len){
    _serial->write(buf, len);
}

