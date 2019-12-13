#include "Gateway.h"
#include "Protocol.h"
#include "Debugger.h"
#include "RobotData.h"
#include "Config.h"

Gateway::Gateway(ITransport* trans){
    _trans = trans;
}

void Gateway::init(){
    Debugger::get()->printf("Initializing gateway ... \n \
            Using protocol %s", VERSION);

    _trans->init();

    _state = WAITING_FOR_HEAD;
    memset(_temp_data, 0, sizeof(_temp_data));
    _checksum = 0;
}

void Gateway::run(){
    unsigned char incoming_byte;
    if (_trans->read(incoming_byte))
        _parse(incoming_byte);
}

void Gateway::_parse(unsigned char c){
    // Debugger::get()->printf("%c", c);
    switch (_state)
    {
    case WAITING_FOR_HEAD:
        if (c == HEAD){
            _state = WAITING_FOR_ID;
            memset(_temp_data, 0, sizeof(_temp_data));
            _temp_id = 0;
            _temp_len = 0;
            _checksum=c;
        }
        break;
    case WAITING_FOR_ID:
        _temp_id = c;
        _state = WAITING_FOR_LEN;
        _checksum+=c;
        break;
    case WAITING_FOR_LEN:
        _temp_len = c;
        _index = 0;
        _state = WAITING_FOR_DATA;
        _checksum+=c;
        break;
    case WAITING_FOR_DATA:
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
        _state = WAITING_FOR_HEAD;
        break;
    }
}

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
    case SET_END_EFFECTOR_STATE:
    {
        RobotData* rd = RobotData::get();
        rd->end_effector_state = _temp_data[0];
    }
        break;
    default:
        break;
    }
}

