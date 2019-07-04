#include <iostream>
#include <event.h>
#include <event2/event-config.h>

void tmcallback(int fd, short event, void *argc) {
    printf("tmcallback...\n");
}

int main() {

    struct event ev;

    struct event_base* base = event_init();
    struct timeval tv;
    tv.tv_sec = 3;
    tv .tv_usec = 0;

    evtimer_set(&ev, tmcallback, NULL);
    event_base_set(base, &ev);

    event_add(&ev, &tv);
    event_base_set(base, &ev);

    event_base_dispatch(base);



    return 0;
}