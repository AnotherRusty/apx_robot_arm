/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#ifndef I_OBSERVER_H_
#define I_OBSERVER_H_


class IObserver
{
public:
    virtual void notify(unsigned char *pArg) = 0;
};


#endif