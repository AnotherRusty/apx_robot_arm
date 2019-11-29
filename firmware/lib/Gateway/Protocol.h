#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define VERSION "V1.0"

#define HEAD    0x5a 
#define MAX_DATA_LEN    255

enum ParseState{
    WAITING_FOR_HEAD = 0,
    WAITING_FOR_ID = 1,
    WAITING_FOR_LEN = 2,
    WAITING_FOR_DATA = 3,
    CHECKSUM =4,
};

enum CommandId{
    DUMMY = 0,
    SET_JOINT_POSITIONS,
    GET_JOINT_POSITIONS,

    MOVE_DONE = 100,
    RET_JOINT_POSITIONS,
};

#endif