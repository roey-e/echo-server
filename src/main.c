#include <stddef.h>
#include <stdio.h>
#include <sys/select.h>

#include "server.h"

#define PORT (1337)

int main()
{
    server_t server;
    fd_set readfds = { 0 };
    fd_set writefds = { 0 };
    fd_set exceptfds = { 0 };

    bool is_triggered = false;

    SERVER_init(&server, PORT);

    while (true) {
        HANDLER_register((handler_t*)&server, &readfds, &writefds);

        select(((handler_t*)&server)->fd + 1, &readfds, &writefds, &exceptfds, NULL);

        HANDLER_is_triggered((handler_t*)&server, &readfds, &writefds, &is_triggered);
        if (is_triggered) {
            HANDLER_handle((handler_t*)&server);
        } else {
            printf("Not triggered\n");
        }
    }

    return 0;
}
