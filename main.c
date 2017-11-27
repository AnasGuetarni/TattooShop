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
#include <ntsid.h>
#include "main_functions.h"
#include "thread_wrapper.h"

int count_threads = 0;

void *thread(void *id_thread) {
    int id = *((int *) id_thread);
    printf(ANSI_COLOR_RED "id thread: %i" ANSI_COLOR_RESET "\n",id);

    printf("Thread %d waiting at barrier...\n", id);
    barrier_wait(&barrier);
    printf("Thread %d passed the barrier!\n", id);

    pthread_mutex_lock(&mutex_salle_attente);
    // section critique
    count_threads++;
    printf(ANSI_COLOR_BLUE "count_threads: %i" ANSI_COLOR_RESET "\n", count_threads);
    // fin section critique
    pthread_mutex_unlock(&mutex_salle_attente);
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

    salleAttente_t listeAttente;
    listeAttente.nombre_seats_available = number_sieges_salle_attente;
    listeAttente.nombre_seats_const= number_sieges_salle_attente;
    listeAttente.client = malloc(sizeof(client_t)*number_sieges_salle_attente);

    int num_threads = number_clients+number_tatoueurs;

    printf(ANSI_COLOR_GREEN "Number of tattoos: %i" ANSI_COLOR_RESET "\n", number_tattoos);
    printf(ANSI_COLOR_GREEN "Number of clients: %i" ANSI_COLOR_RESET "\n", number_clients);
    printf(ANSI_COLOR_GREEN "Number of tattoueurs: %i" ANSI_COLOR_RESET "\n", number_tatoueurs);
    printf(ANSI_COLOR_GREEN "Number of sieges: %i" ANSI_COLOR_RESET "\n", number_sieges_salle_attente);

    printf(ANSI_COLOR_RED "num_threads to create: %i" ANSI_COLOR_RESET "\n",num_threads);

    promenade_t maPromenade;
    maPromenade.walk_min_t = WALK_MIN_T;
    maPromenade.walk_max_t = WALK_MAX_T;

    tattouage_t monTatoo;
    monTatoo.tatoo_min_t = TATOO_MIN_T;
    monTatoo.tatoo_max_t = TATOO_MAX_T;

    printf(ANSI_COLOR_YELLOW "walkMin: %i" ANSI_COLOR_RESET "\n", maPromenade.walk_min_t);
    printf(ANSI_COLOR_YELLOW "walkMax: %i" ANSI_COLOR_RESET "\n", maPromenade.walk_max_t);

    printf(ANSI_COLOR_YELLOW "tatooMin: %i" ANSI_COLOR_RESET "\n", monTatoo.tatoo_min_t);
    printf(ANSI_COLOR_YELLOW "tatooMax: %i" ANSI_COLOR_RESET "\n", monTatoo.tatoo_max_t);

    int result_random_walk = randomWalk(maPromenade);
    int result_random_tatoo = randomTatoo(monTatoo);

    printf(ANSI_COLOR_RED "result_random_walk: %i" ANSI_COLOR_RESET "\n",result_random_walk);
    printf(ANSI_COLOR_RED "result_random_tatoo: %i" ANSI_COLOR_RESET "\n",result_random_tatoo);

    assert(result_random_walk <= maPromenade.walk_max_t && result_random_walk >= maPromenade.walk_min_t);
    assert(result_random_tatoo <= monTatoo.tatoo_max_t && result_random_tatoo >= monTatoo.tatoo_min_t);

    tattoueur_t all_tatoueurs[number_tatoueurs];
    client_t all_clients[number_clients];

    int nombre_seats_per_tattoueurs = number_sieges_salle_attente/number_tatoueurs;
    int reste_nombre_seats_per_tattoueurs = number_sieges_salle_attente%number_tatoueurs;

    for (int i = 0; i<number_tatoueurs;i++){
        all_tatoueurs[i].id_tatoueur = i;
        printf(ANSI_COLOR_CYAN "Id tatoueur: %i" ANSI_COLOR_RESET "\n", all_tatoueurs[i].id_tatoueur);
        if (i == number_tatoueurs)
            all_tatoueurs[i].number_seats = nombre_seats_per_tattoueurs+reste_nombre_seats_per_tattoueurs;
        else
            all_tatoueurs[i].number_seats = nombre_seats_per_tattoueurs;
        printf(ANSI_COLOR_CYAN "Number seats tatoueur: %i" ANSI_COLOR_RESET "\n", all_tatoueurs[i].number_seats);

        all_tatoueurs[i].time_tatoo = randomTatoo(monTatoo);
        printf(ANSI_COLOR_CYAN "Time tatoo: %i" ANSI_COLOR_RESET "\n", all_tatoueurs[i].time_tatoo);
    }

    for (int j = 0; j < number_clients; ++j) {
        all_clients[j].id_client = j;
        printf(ANSI_COLOR_MAGENTA "Id client: %i" ANSI_COLOR_RESET "\n", all_clients[j].id_client);

        all_clients[j].time_promenade = randomWalk(maPromenade);
        printf(ANSI_COLOR_MAGENTA "Time balade: %i" ANSI_COLOR_RESET "\n", all_clients[j].time_promenade);
    }

    if (pthread_mutex_init(&mutex_salle_attente, NULL) != 0)
    {
        fprintf(stderr,"\n mutex init failed\n");
        return 1;
    }

    barrier_init(&barrier, num_threads+1);

    pthread_t threads[num_threads];

    for (int i = 0; i < num_threads; ++i) {
        int code = pthread_create(&threads[i], NULL, thread, &i);

        assert(code == 0);

        if (code != 0) {
            fprintf(stderr, "pthread_create failed!\n");
            return EXIT_FAILURE;
        }
    }

    puts("Thread main waiting at barrier...");
    barrier_wait(&barrier);
    puts("Thread main passed the barrier!");

    for (int k = 0; k < num_threads; ++k) {
        int codeJoin = pthread_join(threads[k], NULL) != 0;

        assert(codeJoin == 0);

        if(codeJoin != 0) {
            printf("pthread_join\n");
            return EXIT_FAILURE;
        }
    }

    pthread_mutex_destroy(&mutex_salle_attente);
    barrier_destroy(&barrier);

    return EXIT_SUCCESS;
}

