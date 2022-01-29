#ifndef _SERVER_H
#define _SERVER_H

#include "handler.h"

typedef struct server_s {
    handler_t base;
} server_t;

bool SERVER_init(server_t* self, int port);

#endif // _SERVER_H
