/*

/mnt/usb/usb2/Drive1 # cat /proc/bus/input/devices 

I: Bus=0003 Vendor=0001 Product=0001 Version=0004
N: Name="LGE RCU"
P: Phys=
S: Sysfs=/class/input/input2
U: Uniq=
H: Handlers=kbd event2 
B: EV=7
B: KEY=7fffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff ffffffff fffffffe
B: REL=0

*/

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>


#define MAX_EVENTS 10
#define MAX_BUF 20


int main(void) {
    int lge_rcu_fd = open("/dev/input/event2", O_RDONLY);
    int epoll_fd;
    struct epoll_event ev;
    struct epoll_event evlist[MAX_EVENTS];

    int i, j, s;
    char buf[MAX_BUF];

    if(lge_rcu_fd == -1) {
        printf("can't open /dev/input/event2");
        exit(EXIT_FAILURE);
    }
    epoll_fd = epoll_create(1);
    if(!epoll_fd == -1) {
        printf("can't create epoll");
        exit(EXIT_FAILURE);
    }

    ev.events = EPOLLIN;  // EPOLLIN for read()
    ev.data.fd = lge_rcu_fd;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, lge_rcu_fd, &ev) == -1) {
        printf("can't create epoll");
        exit(EXIT_FAILURE);
    }

    for(i = 0; i < 10; i++) {
        int ready = epoll_wait(epoll_fd, evlist, MAX_EVENTS, 1000);
        for(j = 0; i < ready; i++) {
            s = read(evlist[j].data.fd, buf, MAX_BUF);
            printf("event buf %s", buf);
        }
    }

}
