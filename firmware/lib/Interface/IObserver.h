#ifndef I_OBSERVER_H_
#define I_OBSERVER_H_


class IObserver
{
public:
    virtual void notify(unsigned char *pArg) = 0;
};


#endif