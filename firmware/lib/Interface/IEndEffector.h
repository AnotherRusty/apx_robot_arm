#ifndef I_END_EFFECTOR_H_
#define I_END_EFFECTOR_H_

// interface

class IEndEffector
{
public:
    virtual void init() = 0;
    virtual void grip() = 0;
    virtual void release() = 0;
};


#endif
