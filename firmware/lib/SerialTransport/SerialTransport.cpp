/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#include "SerialTransport.h"
#include <Arduino.h>
#include "Debugger.h"


void serialEvent(){

}

// 构造函数 
// 参数： ser        HardwareSerial对象，具体Arduino的serial
//       baudrate   波特率
SerialTransport::SerialTransport(HardwareSerial& ser, uint32_t baudrate){
    _serial = &ser;
    _baudrate = baudrate;
}

// 初始化
// 参数： None
// 返回值： None
void SerialTransport::init(){
    Debugger::get()->printf("Serial transport begin at %ld hz.", _baudrate);
    _serial->begin(_baudrate);
}

// read，读取串口数据
// 参数： ch    unsigned char接受一个字节
// 返回值： bool    接受到true/没有数据false
bool SerialTransport::read(unsigned char& ch){
    if (_serial->available()){
        ch = _serial->read();
        return true;
    }
    return false;
}

// write，发送数据
// 参数： buf    发送的数据buf
//       len    发送的字节数
// 返回值： None
void SerialTransport::write(unsigned char* buf, unsigned char len){
    _serial->write(buf, len);
}

