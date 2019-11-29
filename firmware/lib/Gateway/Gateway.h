#ifndef GATEWAY_H_
#define GATEWAY_H_

#include "Protocol.h"
#include "ITransport.h"
#include "Config.h"

class Gateway
{
public:
    Gateway(ITransport* trans);
    ~Gateway(){};
    void init();
    void run();
    void make_response(uint8_t id, unsigned char* buf, uint8_t &len);
    
private:
    void _parse(unsigned char c);
    void _ressolve_message(uint8_t msg_id); 

    ITransport* _trans;
    ParseState  _state;
    uint8_t _temp_id;
    uint8_t _temp_len;
    unsigned char _temp_data[MAX_DATA_LEN];
    unsigned char _checksum;
    uint8_t _index;

};


#endif