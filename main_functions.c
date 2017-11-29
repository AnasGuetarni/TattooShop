//
// Created by Anas Guetarni on 22/11/2017.
//

#include <stdlib.h>
#include "main_functions.h"
#include "thread_wrapper.h"

int randomWalk(int value_min, int value_max){
    return rand()%(value_max-value_min)+value_min;
}

int randomTatoo(int value_min, int value_max){
    return rand()%(value_max-value_min)+value_min;
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
