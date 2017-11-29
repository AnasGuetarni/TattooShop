//
// Created by Anas Guetarni on 22/11/2017.
//

#include <stdlib.h>
#include "main_functions.h"
#include "thread_wrapper.h"

int randomWalk(promenade_t unePromenade){
    return rand()%(unePromenade.walk_max_t-unePromenade.walk_min_t)+unePromenade.walk_min_t;
}

int randomTatoo(tattouage_t unTatoo){
    return rand()%(unTatoo.tatoo_max_t-unTatoo.tatoo_min_t)+unTatoo.tatoo_min_t;
}

/*
void barrier_init(barrier_t *b, int count) {
    b->count = count;
    b->mutex = mutex_create();
    b->block = mutex_create();
}

void barrier_wait(barrier_t *b) {
    mutex_lock(b->mutex);
    b->count--;
    mutex_unlock(b->mutex);

    mutex_lock(b->block);
    while (b->count) {
        sched_yield();
    }
    mutex_unlock(b->block);
}

void barrier_destroy(barrier_t *b) {
    mutex_destroy(b->mutex);
    mutex_destroy(b->block);
}
 */
