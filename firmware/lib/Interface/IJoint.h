#ifndef I_JOINT_H_
#define I_JOINT_H_


// interface


class IJoint{

public:
    virtual void init() = 0;
    virtual void move(unsigned short angle) = 0;
    virtual void direct_move(unsigned short angle) = 0;
    virtual unsigned short get_position() = 0;
};


#endif