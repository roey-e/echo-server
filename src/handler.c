#include "handler.h"

bool HANDLER_init(handler_t* self,
    int fd,
    can_read_f* can_read,
    can_write_f* can_write,
    handle_f* handle)
{
    self->fd = fd;
    self->can_read = can_read;
    self->can_write = can_write;
    self->handle = handle;

    return true;
}
bool HANDLER_register(handler_t* self, fd_set* readfds, fd_set* writefds)
{
    bool can_read = false;
    bool can_write = false;

    if (!self->can_read(self, &can_read)) {
        return false;
    }
    if (!self->can_write(self, &can_write)) {
        return false;
    }

    if (can_read) {
        FD_SET(self->fd, readfds);
    }
    if (can_write) {
        FD_SET(self->fd, writefds);
    }

    return true;
}

bool HANDLER_is_triggered(handler_t* self,
    const fd_set* readfds,
    const fd_set* writefds,
    bool* result)
{
    *result = FD_ISSET(self->fd, readfds) | FD_ISSET(self->fd, writefds);

    return true;
}

bool HANDLER_handle(handler_t* self)
{
    if (0 != self->handle(self)) {
        return false;
    }

    return true;
}
