/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#ifndef GATEWAY_H_
#define GATEWAY_H_

#include "Protocol.h"
#include "ITransport.h"
#include "Config.h"

/* Gateway类
负责解析消息及生成返回消息，具体的消息收发通过ITransport成员对象
Gateway会从ITransport中读取消息 （每次循环读取1个字节）放入Gateway::_parse()函数中解析，
如果解析到一条完整的消息则调用Gateway::_ressolve_message()进行具体消息的解析。
*/
class Gateway
{
public:
    Gateway(ITransport* trans); //构造是需要传入ITransport对象
    ~Gateway(){};
    void init();    //初始化
    void run();     //主函数
    void make_response(uint8_t id, unsigned char* buf, uint8_t &len);   //生成返回消息
    
private:
    void _parse(unsigned char c);
    void _ressolve_message(uint8_t msg_id); 

    ITransport* _trans; //ITransport指针，指向传入的ITransport对象
    ParseState  _state; //记录当前的解析状态
    uint8_t _temp_id;   //保存当前收到的消息id
    uint8_t _temp_len;  //保存当前收到的消息长度
    unsigned char _temp_data[MAX_DATA_LEN]; //保存当前收到的消息数据
    unsigned char _checksum;    //保存当前收到的校验和
    uint8_t _index;

};


#endif