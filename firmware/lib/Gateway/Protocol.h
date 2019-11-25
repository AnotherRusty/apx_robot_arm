/* OPENARM Firmware *****************
    -Arm model: Victor
    -Version 1.0

    Danny @ openarm 2019
************************************/
#ifndef PROTOCOL_H_
#define PROTOCOL_H_

/* Protocol定义了协议相关的一些东西 
    消息组成： |消息头|ID|数据长度|数据|校验|
*/

#define VERSION "V1.0"

#define HEAD    0x5a    // 消息头
#define MAX_DATA_LEN    255 //最大消息长度 255字节

//消息解析状态
enum ParseState{
    WAITING_FOR_HEAD = 0,
    WAITING_FOR_ID = 1,
    WAITING_FOR_LEN = 2,
    WAITING_FOR_DATA = 3,
    CHECKSUM =4,
};

//消息ID， 1-99为下发命令（上位机-->单片机）， 100以上是返回消息（单片机-->上位机)
enum CommandId{
    DUMMY = 0,
    SET_JOINT_POSITIONS,    //设置关节角度 ID=1
    GET_JOINT_POSITIONS,    //获取关节角度 ID=2
    //在此添加自定义命令

    MOVE_DONE = 100,
    RET_JOINT_POSITIONS,    //关节角度返回 ID=101
    //在此添加返回消息
};

#endif