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


void *client(void *params) {
    param_t *param = (param_t*) params;

    int id = param->id_thread;

    printf(ANSI_COLOR_RED "id thread: %i" ANSI_COLOR_RESET "\n",param->id_thread);
    printf(ANSI_COLOR_RED "seats available: %i" ANSI_COLOR_RESET "\n",param->sale_attente.nombre_seats_available);
    printf(ANSI_COLOR_RED "seats const: %i" ANSI_COLOR_RESET "\n",param->sale_attente.nombre_seats_const);
    printf("id client in thread: %i\n",param->sale_attente.client[id].id_client);
    printf("time client in thread: %i\n",param->sale_attente.client[id].time_promenade);
    printf("id tatoueur in thread: %i\n",param->sale_attente.tattoueurs[id].id_tatoueur);
    printf("time tatoueur in thread: %i\n",param->sale_attente.tattoueurs[id].time_tatoo);

    param->sale_attente.tattoueurs[id] = param->sale_attente.tattoueurs[param->sale_attente.nombre_seats_const-param->sale_attente.nombre_seats_available];
    printf("Tatoueur id: %i\n", param->sale_attente.tattoueurs[id]);

    struct timespec ts;
    ts.tv_sec = param->sale_attente.client[id].time_promenade / 100;
    ts.tv_nsec = 0;

    struct timespec ts2;
    ts.tv_sec = param->sale_attente.tattoueurs[id].time_tatoo / 10;
    ts.tv_nsec = 0;

    printf("C'est partit pour une balade pour le client %i\n", param->sale_attente.client[id].id_client);

    if (nanosleep(&ts, NULL) != 0){
        fprintf(stderr, "Nanosleep error");
        return EXIT_FAILURE;
    }

    do {
        pthread_mutex_lock(&mutex_salle_attente);
        param->sale_attente.nombre_seats_available--;
        pthread_mutex_unlock(&mutex_salle_attente);

        if (param->sale_attente.nombre_seats_available > 0){
            // On laisse le temps au tattoueur de tatouer
            printf("Le client %i se fait hagar par le tatoueur %i\n", param->sale_attente.client[id].id_client, param->sale_attente.tattoueurs[id].id_tatoueur);

            if (nanosleep(&ts2, NULL) != 0){
                fprintf(stderr, "Nanosleep error");
                return EXIT_FAILURE;
            }
        }
        else
            printf("Le client %i ne se fait pas hagar par le tatoueur %i\n", param->sale_attente.client[id].id_client, param->sale_attente.tattoueurs[id].id_tatoueur);

        printf("Le client %i a finit de se faire hagar par le tatoueur %i\n", param->sale_attente.client[id].id_client, param->sale_attente.tattoueurs[id].id_tatoueur);

        pthread_mutex_lock(&mutex_salle_attente);
        param->sale_attente.nombre_seats_available++;
        pthread_mutex_unlock(&mutex_salle_attente);

    } while (param->sale_attente.nombre_seats_available <= 0);

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

    printf(ANSI_COLOR_GREEN "Number of tattoos: %i" ANSI_COLOR_RESET "\n", number_tattoos);
    printf(ANSI_COLOR_GREEN "Number of clients: %i" ANSI_COLOR_RESET "\n", number_clients);
    printf(ANSI_COLOR_GREEN "Number of tattoueurs: %i" ANSI_COLOR_RESET "\n", number_tatoueurs);
    printf(ANSI_COLOR_GREEN "Number of sieges: %i" ANSI_COLOR_RESET "\n", number_sieges_salle_attente);

    printf(ANSI_COLOR_RED "num_threads to create: %i" ANSI_COLOR_RESET "\n",number_clients+number_tatoueurs);

    salleAttente_t listeAttente;
    listeAttente.nombre_seats_available = number_sieges_salle_attente;
    listeAttente.nombre_seats_const= number_sieges_salle_attente;
    listeAttente.client = malloc(sizeof(client_t)*number_sieges_salle_attente);

    assert(randomWalk(WALK_MIN_T,WALK_MAX_T) <= WALK_MAX_T && randomWalk(WALK_MIN_T,WALK_MAX_T) >= WALK_MIN_T);
    assert(randomTatoo(TATOO_MIN_T, TATOO_MAX_T) <= TATOO_MAX_T && randomTatoo(TATOO_MIN_T, TATOO_MAX_T) >= TATOO_MIN_T);

    tattoueur_t *all_tatoueurs = malloc(sizeof(client_t)*number_tatoueurs);
    client_t *all_clients = malloc(sizeof(client_t)*number_clients);

    for (int i = 0; i<number_tatoueurs;i++){
        all_tatoueurs[i].id_tatoueur = i;
        printf(ANSI_COLOR_CYAN "Id tatoueur: %i" ANSI_COLOR_RESET "\n", all_tatoueurs[i].id_tatoueur);

        all_tatoueurs[i].time_tatoo = randomTatoo(TATOO_MIN_T, WALK_MAX_T);
        printf(ANSI_COLOR_CYAN "Time tatoo: %i" ANSI_COLOR_RESET "\n", all_tatoueurs[i].time_tatoo);
    }

    for (int j = 0; j < number_clients; j++) {
        all_clients[j].id_client = j;
        printf(ANSI_COLOR_MAGENTA "j client: %i" ANSI_COLOR_RESET "\n", j);
        printf(ANSI_COLOR_MAGENTA "Id client: %i" ANSI_COLOR_RESET "\n", all_clients[j].id_client);

        all_clients[j].time_promenade = randomWalk(WALK_MIN_T,WALK_MAX_T);
        printf(ANSI_COLOR_MAGENTA "Time balade: %i" ANSI_COLOR_RESET "\n", all_clients[j].time_promenade);
    }

    listeAttente.client = all_clients;
    listeAttente.tattoueurs = all_tatoueurs;


    pthread_t threads_clients[number_clients];
    param_t params;

    for (int k = 0; k < number_clients; ++k) {
        params.sale_attente.client = listeAttente.client;
    }
    for (int l = 0; l < number_tatoueurs; ++l) {
        params.sale_attente.tattoueurs = listeAttente.tattoueurs;
    }


    for (int i = 0; i < number_clients; ++i) {
        params.id_thread = i;
        params.sale_attente.nombre_seats_available = listeAttente.nombre_seats_available;
        params.sale_attente.nombre_seats_const = listeAttente.nombre_seats_const;

        int code = pthread_create(&threads_clients[i], NULL, client, &params);

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

    pthread_mutex_destroy(&mutex_salle_attente);

    return EXIT_SUCCESS;
}

