#ifndef TARGET_GROUP_H
#define TARGET_GROUP_H

#include <pthread.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#include "./config.h"
struct target_group_list_node {
    struct target_group tg;
    struct target_group_list_node *next;
};

void target_group_list_sorted_insert(struct target_group target_group);

void find_target_group_with_path(char *path, struct target_group **tg);
#endif