#ifndef _HANDLER_H
#define _HANDLER_H

#include <stdbool.h>
#include <sys/select.h>

typedef struct _handler_s handler_t;

typedef bool
can_read_f(handler_t* self, bool* result);
typedef bool
can_write_f(handler_t* self, bool* result);
typedef bool
handle_f(handler_t* self);

struct _handler_s {
    int fd;

    can_read_f* can_read;
    can_write_f* can_write;
    handle_f* handle;
};

bool HANDLER_init(handler_t* self,
    int fd,
    can_read_f* can_read,
    can_write_f* can_write,
    handle_f* handle);
bool HANDLER_register(handler_t* self, fd_set* readfds, fd_set* writefds);
bool HANDLER_is_triggered(handler_t* self,
    const fd_set* readfds,
    const fd_set* writefds,
    bool* result);
bool HANDLER_handle(handler_t* self);

#endif // _HANDLER_H
