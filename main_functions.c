//
// Created by Anas Guetarni on 22/11/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>
#include <time.h>
#include "main_functions.h"
#include "thread_wrapper.h"

int randomWalk(int value_min, int value_max){
    return rand()%(value_max-value_min)+value_min;
}

int randomTatoo(int value_min, int value_max){
    return rand()%(value_max-value_min)+value_min;
}
