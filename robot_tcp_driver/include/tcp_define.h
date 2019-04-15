//
// Created by ray on 19-3-31.
//
#include <stdlib.h>
#ifndef AGV_TCP_DRIVER_TCP_DEFINE_H
#define AGV_TCP_DRIVER_TCP_DEFINE_H

#define MAX_CMD_V_LINE 1.0
#define MAX_CMD_V_ANGLE 1.0
typedef struct {
    //u_int8_t direct_cmd ;
    int8_t direct_cmd_v_line;  //-127~+127
    int8_t direct_cmd_v_angle; //-127~+127
} RecvContainer;

union MapHeadUnion
{
    int32_t at;
    char tab[4];
};
typedef struct{
    MapHeadUnion width;
    MapHeadUnion height;

}MapHead;
typedef struct {
    MapHead map_head;
    void *  data_ptr;
} MapMessage;

enum DirectCmd{
    UP = 1,
    DOWN,
    LEFT,
    RIGHT,
    STOP
};

bool Take(RecvContainer *recv_container);
bool Send(MapMessage &send_map_message);

#endif //AGV_TCP_DRIVER_TCP_DEFINE_H
