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
#include "main_functions.h"

void *thread(void *args) {

    return EXIT_SUCCESS;
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

    printf("num_threads: %i",num_threads);

    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        int code = pthread_create(&threads[i], NULL, thread, NULL);

        if (code != 0) {
            fprintf(stderr, "pthread_create failed!\n");
            return EXIT_FAILURE;
        }

        if(pthread_join(threads[i], NULL) != 0) {
            printf("pthread_join\n");
            return EXIT_FAILURE;
        }
    }

    return EXIT_SUCCESS;
}

