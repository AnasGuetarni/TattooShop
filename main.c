//
//  main.c
//  Serie 1 - Exercice 3 - Thread
//
//  Created by Anas Guetarni on 28/09/2017.
//  Copyright © 2017 Anas Guetarni. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include "main_functions.h"

#define WALK_MIN_T 30
#define WALK_MAX_T 50
#define TATOO_MIN_T 5
#define TATOO_MAX_T 10

typedef struct promenadeT {
    int walk_min_t;
    int walk_max_t;
} promenade_t;

typedef struct tattouageT {
    int tatoo_min_t;
    int tatoo_max_t;
} tattouage_t;

void *thread(void *args) {

    return EXIT_SUCCESS;
}

int randomWalk(promenade_t unePromenade){
    return rand()%(unePromenade.walk_max_t-unePromenade.walk_min_t)+unePromenade.walk_min_t;
}

int randomTatoo(tattouage_t unTatoo){
    return rand()%(unTatoo.tatoo_max_t-unTatoo.tatoo_min_t)+unTatoo.tatoo_min_t;
}

int main(int argc, const char * argv[]) {

    if (argc < 4){
        fprintf(stderr,"Not enought arguments");
        return EXIT_FAILURE;
    }

    int number_tattoos = atoi(argv[1]);
    int number_clients = atoi(argv[2]);
    int number_tatoueurs = atoi(argv[3]);
    int number_sieges = atoi(argv[4]);

    int num_threads = number_clients+number_tatoueurs;

    printf("Number of tattoos: %i\n", number_tattoos);
    printf("Number of clients: %i\n", number_clients);
    printf("Number of tattoueurs: %i\n", number_tatoueurs);
    printf("Number of sieges: %i\n", number_sieges);

    printf("num_threads to create: %i\n",num_threads);

    promenade_t maPromenade;
    maPromenade.walk_min_t = WALK_MIN_T;
    maPromenade.walk_max_t = WALK_MAX_T;

    tattouage_t monTatoo;
    monTatoo.tatoo_min_t = TATOO_MIN_T;
    monTatoo.tatoo_max_t = TATOO_MAX_T;

    printf("walkMin: %i\n", maPromenade.walk_min_t);
    printf("walkMax: %i\n", maPromenade.walk_max_t);

    printf("tatooMin: %i\n", monTatoo.tatoo_min_t);
    printf("tatooMax: %i\n", monTatoo.tatoo_max_t);

    int result_random_walk = randomWalk(maPromenade);
    int result_random_tatoo = randomTatoo(monTatoo);

    printf("result_random_walk: %i\n",result_random_walk);
    printf("result_random_tatoo: %i\n",result_random_tatoo);

    assert(result_random_walk <= maPromenade.walk_max_t && result_random_walk >= maPromenade.walk_min_t);
    assert(result_random_tatoo <= monTatoo.tatoo_max_t && result_random_tatoo >= monTatoo.tatoo_min_t);

    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        int code = pthread_create(&threads[i], NULL, thread, NULL);

        assert(code == 0);

        if (code != 0) {
            fprintf(stderr, "pthread_create failed!\n");
            return EXIT_FAILURE;
        }

        int codeJoin = pthread_join(threads[i], NULL) != 0;

        assert(codeJoin == 0);

        if(codeJoin != 0) {
            printf("pthread_join\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

