#include "server.h"

#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_LISTEN_QUEUE (5)

can_read_f _SERVER_can_read;
can_write_f _SERVER_can_write;
handle_f _SERVER_handle;

bool SERVER_init(server_t* self, int port)
{
    int listen_socket = -1;
    struct sockaddr_in listen_address = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(port),
        .sin_zero = { 0 },
    };

    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (-1 == listen_socket) {
        return false;
    }

    if (0 != bind(listen_socket, (struct sockaddr*)&listen_address, sizeof(listen_address))) {
        return false;
    }

    if (0 != listen(listen_socket, MAX_LISTEN_QUEUE)) {
        return false;
    }

    if (0 != HANDLER_init(&self->base, listen_socket, &_SERVER_can_read, &_SERVER_can_write, &_SERVER_handle)) {
        return false;
    }

    return true;
}

bool _SERVER_can_read(handler_t* self, bool* result)
{
    *result = true;

    return true;
}

bool _SERVER_can_write(handler_t* self, bool* result)
{
    *result = false;

    return true;
}

bool _SERVER_handle(handler_t* self)
{
    int connection_fd = -1;
    struct sockaddr_in connection_address = { 0 };
    socklen_t address_length = sizeof(connection_address);

    connection_fd = accept(self->fd, (struct sockaddr*)&connection_address, &address_length);

    send(connection_fd, "bye!\n", 5, 0);
    close(connection_fd);

    return true;
}
