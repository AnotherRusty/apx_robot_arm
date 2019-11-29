#ifndef I_TRANSPORT_H_
#define I_TRANSPORT_H_


// interface

#include <Arduino.h>

class ITransport
{
public:
    virtual void init() = 0;
    virtual bool read(unsigned char& ch) = 0;
    virtual void write(unsigned char* buf, unsigned char len) = 0;
};


#endif