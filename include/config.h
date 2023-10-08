#ifndef CONFIG_H
#define CONFIG_H

#include <arpa/inet.h>
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./env.h"

// setting up a reverse proxy
struct target_backend {
    char name[100];
    char host[INET6_ADDRSTRLEN];
    unsigned int port;
    int is_healthy; // 0 - unhealthy, 1- healthy
};

struct round_robin_node {
    struct target_backend backend;
    struct round_robin_node *next;
};

struct target_group {
    /**
    * Target groups are a group of targets that are linked to a particular path of the load balancer.
    * For a request, the load balancer will first go through the paths according to the priority and regex match them.
    * 1. If none of the paths match, then the request is routed to the default route. 
    * 2. Priorities are descending. 
    * 3. If all the target groups have the same priority, then the priority is decided internally by the insertion index. That means, the target groups inserted first will have an higher priority.
    * 4. If no default target group is mentioned, then the highest priority target group is used as the default one
     **/
    char path[100];
    int priority;
    struct round_robin_node *round_robin_head;
    struct round_robin_node *round_robin_current;
    int is_default;
};
#endif
//CONFIG_H