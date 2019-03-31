//
// Created by ray on 19-3-31.
//
#include <stdlib.h>
#ifndef AGV_TCP_DRIVER_TCP_DEFINE_H
#define AGV_TCP_DRIVER_TCP_DEFINE_H

typedef struct {
    u_int8_t direct_cmd ;
} RecvContainer;

bool Take(RecvContainer *recv_container);

enum DirectCmd{
    UP = 1,
    DOWN,
    LEFT,
    RIGHT,
    STOP
};


#endif //AGV_TCP_DRIVER_TCP_DEFINE_H
