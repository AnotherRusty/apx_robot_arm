/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#include "Gateway.h"
#include "Protocol.h"
#include "Debugger.h"
#include "RobotData.h"
#include "Config.h"

// Gateway构造函数
// 参数： trans   ITransport对象指针
Gateway::Gateway(ITransport* trans){
    //_trans指针指向传入的ITransport
    _trans = trans;
}

// 初始化
// 参数： None
// 返回值： None
void Gateway::init(){
    Debugger::get()->printf("Initializing gateway ... \n \
            Using protocol %s", VERSION);

    _trans->init(); //初始化tranport

    _state = WAITING_FOR_HEAD;  //重置解析状态
    memset(_temp_data, 0, sizeof(_temp_data));  //清空临时数据
    _checksum = 0;
}

// 主函数，被程序主循环调用
// 参数： None
// 返回值： None
void Gateway::run(){
    unsigned char incoming_byte;
    //从transport中读取1个字节数据，如果有数据则调用_parse()解析
    if (_trans->read(incoming_byte))
        _parse(incoming_byte);
}

// _parse
// 参数： c 收到的字节
// 返回值： None
void Gateway::_parse(unsigned char c){
    // Debugger::get()->printf("%c", c);
    switch (_state)
    {
    case WAITING_FOR_HEAD:
    //如果收到消息头（0x5a)，重置变量准备接受新的一条消息，解析状态变为WAITING_FOR_ID，_checksum累加
        if (c == HEAD){
            _state = WAITING_FOR_ID;
            memset(_temp_data, 0, sizeof(_temp_data));
            _temp_id = 0;
            _temp_len = 0;
            _checksum=c;
        }
        break;
    case WAITING_FOR_ID:
    //收到的字节保存到_temp_id，解析状态变为WAITING_FOR_LEN，_checksum累加
        _temp_id = c;
        _state = WAITING_FOR_LEN;
        _checksum+=c;
        break;
    case WAITING_FOR_LEN:
    //收到的字节保存到_temp_len，解析状态变为WAITING_FOR_DATA，_checksum累加
        _temp_len = c;
        _index = 0; // 重置索引，代表当前准备接收第1（n+1)个消息数据字节
        _state = WAITING_FOR_DATA;
        _checksum+=c;
        break;
    case WAITING_FOR_DATA:
    //收到的字节保存到_temp_data，如果消息数据已经符合消息长度则进行checksum检验，如果校验成功调用_ressolve_message，
    //重置解析状态为WAITING_FOR_HEAD
        if ((_temp_len--)>0){
            _temp_data[_index++] = c;
            _checksum+=c;
        }
        else{
            _state = CHECKSUM;
            if(c==_checksum){
                _ressolve_message(_temp_id);
            }
            _state = WAITING_FOR_HEAD;
        }
        break;

    default:
    //默认设置解析状态为WAITING_FOR_HEAD
        _state = WAITING_FOR_HEAD;
        break;
    }
}

// make_response
// 参数： id    返回消息的id
//       buf   返回消息buffer
//       len   消息的长度
// 返回值： None
void Gateway::make_response(uint8_t id, unsigned char* buf, uint8_t &len){
    buf[0] = HEAD;
    buf[1] = id;
    switch (id)
    {
    case RET_JOINT_POSITIONS:
    {
        // Debugger::get()->printf("Respond: id = %d", RET_JOINT_POSITIONS);
        RobotData* rd = RobotData::get();
        unsigned char length = sizeof(rd->current_joint_angles);
        buf[2] = length;
        memcpy(buf+3, rd->current_joint_angles, length);
        
        unsigned char checksum = 0;
        for(int i=0; i<3+length; i++){
            checksum+=buf[i];
        }
        buf[3+length] = checksum&0xff;
        len = 4+length;
    }
        break;
    default:
        break;
    }
}

// _ressolve_message
// 参数：   msg_id  收到消息的id
// 返回值： None
void Gateway::_ressolve_message(uint8_t msg_id){
    // Debugger::get()->printf("In message: id = %d", msg_id);

    switch (msg_id)
    {
    case DUMMY:
        break;
    case SET_JOINT_POSITIONS:
    {
        RobotData* rd = RobotData::get();
        memcpy(rd->target_joint_angles, _temp_data, sizeof(rd->target_joint_angles));
    }
        break;
    case GET_JOINT_POSITIONS:
    {
        unsigned char buf[MAX_DATA_LEN];
        uint8_t len;
        uint8_t res_id = RET_JOINT_POSITIONS;
        make_response(res_id, buf, len);
        _trans->write(buf, len);
    }
        break;
    default:
        break;
    }
}

