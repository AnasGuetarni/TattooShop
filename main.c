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
#include "thread_wrapper.h"

int nombre_tattoo_eff = 0;

void *salle_attente(void *params) {
    param_t *param = (param_t*) params;

    sem_wait(&sem_porte);
    sem_wait(&sem_seats);
    sem_post(&sem_porte);
    sem_wait(&sem_fauteuils);
    sem_post(&sem_start_tattoo);
    sem_wait(&sem_end_tattoo);
    sem_post(&sem_fauteuils);

    promenade(param);

    return EXIT_SUCCESS;
}

void *promenade(void *params){
    param_t *param = (param_t*) params;

    struct timespec ts;
    ts.tv_sec = randomWalk(WALK_MIN_T, WALK_MAX_T);
    ts.tv_nsec = 0;

    if (nanosleep(&ts, NULL) != 0){
        fprintf(stderr, "Nanosleep error");
        return EXIT_FAILURE;
    }

    salle_attente(param);

    return EXIT_SUCCESS;
}


void *tattouages (void *params){
    param_t *param = (param_t*) params;

    do{
        pthread_mutex_lock(&mut_start_tattoo);
        sem_wait(&sem_start_tattoo);
        sem_wait(&sem_start_tattoo);
        pthread_mutex_unlock(&mut_start_tattoo);

        struct timespec ts;
        ts.tv_sec = randomTatoo(TATOO_MIN_T, TATOO_MAX_T);
        ts.tv_nsec = 0;

        if (nanosleep(&ts, NULL) != 0){
            fprintf(stderr, "Nanosleep error");
            return EXIT_FAILURE;
        }

        pthread_mutex_lock(&mut_tattoo_eff);
        nombre_tattoo_eff++;
        pthread_mutex_unlock(&mut_tattoo_eff);

        if (param->nombre_tatoos == nombre_tattoo_eff){
            printf("Tous les tattouages ont été réalisés");
            return EXIT_SUCCESS;
        }

        sem_post(&sem_end_tattoo);

    } while (param->nombre_tatoos != nombre_tattoo_eff);

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
    int number_sieges_salle_attente = atoi(argv[4]);

    sem_init(&sem_porte,0,0);
    sem_init(&sem_seats,0,0);
    sem_init(&sem_fauteuils,0,0);
    sem_init(&sem_start_tattoo,0,0);
    sem_init(&sem_end_tattoo,0,0);

    pthread_mutex_init(&mut_start_tattoo,NULL);
    pthread_mutex_init(&mut_tattoo_eff,NULL);

    printf(ANSI_COLOR_GREEN "Number of tattoos: %i" ANSI_COLOR_RESET "\n", number_tattoos);
    printf(ANSI_COLOR_GREEN "Number of clients: %i" ANSI_COLOR_RESET "\n", number_clients);
    printf(ANSI_COLOR_GREEN "Number of tattoueurs: %i" ANSI_COLOR_RESET "\n", number_tatoueurs);
    printf(ANSI_COLOR_GREEN "Number of sieges: %i" ANSI_COLOR_RESET "\n", number_sieges_salle_attente);

    printf(ANSI_COLOR_RED "num_threads to create: %i" ANSI_COLOR_RESET "\n",number_clients+number_tatoueurs);

    assert(randomWalk(WALK_MIN_T,WALK_MAX_T) <= WALK_MAX_T && randomWalk(WALK_MIN_T,WALK_MAX_T) >= WALK_MIN_T);
    assert(randomTatoo(TATOO_MIN_T, TATOO_MAX_T) <= TATOO_MAX_T && randomTatoo(TATOO_MIN_T, TATOO_MAX_T) >= TATOO_MIN_T);

    pthread_t threads_clients[number_clients];
    pthread_t threads_tattoo[number_clients];
    param_t params_salle_attente;

    for (int k = 0; k < number_clients; ++k) {
        params_salle_attente.client[k].id_client = k;
        params_salle_attente.client[k].time_promenade = randomWalk(WALK_MIN_T,WALK_MAX_T);
    }
    for (int l = 0; l < number_tatoueurs; ++l) {
        params_salle_attente.tattoueurs[l].id_tatoueur = l;
        params_salle_attente.tattoueurs[l].time_tatoo = randomTatoo(TATOO_MIN_T,TATOO_MAX_T);
    }


    for (int i = 0; i < number_clients; ++i) {
        params_salle_attente.id_thread = i;
        params_salle_attente.nombre_tatoos = number_tattoos;
        params_salle_attente.nombre_seats_available = number_sieges_salle_attente;
        params_salle_attente.nombre_seats_const = number_sieges_salle_attente;

        int code = pthread_create(&threads_clients[i], NULL, promenade, &params_salle_attente);

        assert(code == 0);

        if (code != 0) {
            fprintf(stderr, "pthread_create failed!\n");
            return EXIT_FAILURE;
        }

        int codeJoin = pthread_join(threads_clients[i], NULL) != 0;

        assert(codeJoin == 0);

        if(codeJoin != 0) {
            printf("pthread_join\n");
            return EXIT_FAILURE;
        }
    }

    for (int i = 0; i < number_tatoueurs; ++i) {
        params_salle_attente.id_thread = i;
        params_salle_attente.nombre_tatoos = number_tattoos;
        params_salle_attente.nombre_seats_available = number_sieges_salle_attente;
        params_salle_attente.nombre_seats_const = number_sieges_salle_attente;

        int code = pthread_create(&threads_tattoo[i], NULL, tattouages, &params_salle_attente);

        assert(code == 0);

        if (code != 0) {
            fprintf(stderr, "pthread_create failed!\n");
            return EXIT_FAILURE;
        }

        int codeJoin = pthread_join(threads_tattoo[i], NULL) != 0;

        assert(codeJoin == 0);

        if(codeJoin != 0) {
            printf("pthread_join\n");
            return EXIT_FAILURE;
        }
    }

    sem_destroy(&sem_porte);
    sem_destroy(&sem_seats);
    sem_destroy(&sem_fauteuils);
    sem_destroy(&sem_start_tattoo);
    sem_destroy(&sem_end_tattoo);

    pthread_mutex_destroy(&mut_start_tattoo);
    pthread_mutex_destroy(&mut_tattoo_eff);

    return EXIT_SUCCESS;
}

