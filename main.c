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
#include <semaphore.h>
#include <time.h>
#include "main_functions.h"
#include "thread_wrapper.h"

int nombre_tattoo_eff = 0;

void *promenade(void *params){
    param_t *param = (param_t*) params;

    printf("Le thread %i rentre en promenade\n", param->id_thread);

  	struct timespec ts;
  	ts.tv_sec = randomWalk(WALK_MIN_T, WALK_MAX_T) / 10;
  	ts.tv_nsec = 0;

  	assert(nanosleep(&ts, NULL) == 0);

    printf("Le thread %i a finit sa promenade\n", param->id_thread);
    printf("Le thread %i va rentrer alors en salle d'attente\n", param->id_thread);

  	salle_attente(param);

    return EXIT_SUCCESS;
}

void *salle_attente(void *params) {
    param_t *param = (param_t*) params;

    printf("Le thread %i rentre en salle d'attente\n", param->id_thread);

	pthread_mutex_lock(&promenadance);
  printf("On enleve un siege disponible\n");
	param->nombre_siege_disponible--;

	if (param->nombre_siege_disponible < 0)
    {
      printf("Et c'est partit pour une promenade sur le thread : %i car aucun siège n'est disponible\n", param->id_thread);
		  pthread_mutex_unlock(&promenadance);
		  promenade(param);
	}

    /* Porte d'entrée */
    sem_wait(&sem_porte);
    sem_wait(&sem_seats);
    sem_post(&sem_porte);

    sem_wait(&sem_fauteuils); // Regarde si un tatoueur est disponible
    sem_post(&sem_seats); // Libère le siège qu'il occupait

    sem_post(&sem_start_tattoo);  // Lance un tatoueur

    sem_wait(&sem_end_tattoo); // attend la fin de son tattoo

    sem_post(&sem_fauteuils);  // libère le fauteuil ou il s'est fait tattouer

    promenade(param); // Retourne se promener

    return EXIT_SUCCESS;
}

void *tattoueur (void *params){
    param_t *param = (param_t*) params;

    printf("Le tattoueur rentre dans son entre\n");

    do{
        pthread_mutex_lock(&tattoueur_reveil);
        sem_wait(&sem_start_tattoo); // Attend un client
        sem_wait(&sem_start_tattoo); // Referme pour le prochain thread
        pthread_mutex_unlock(&tattoueur_reveil);

        printf("Le tatouage va commencer pour le thread %i\n", param->id_thread);

        struct timespec ts;
        ts.tv_sec = randomTatoo(TATOO_MIN_T, TATOO_MAX_T) / 10;
        ts.tv_nsec = 0;

        assert(nanosleep(&ts, NULL) != 0);

        pthread_mutex_lock(&mut_tattoo_eff);
        nombre_tattoo_eff++;
        pthread_mutex_unlock(&mut_tattoo_eff);

        printf("Le tattouage a bien été effectué\n");

        if (param->nombre_tatoos == nombre_tattoo_eff){
            printf("Tous les tattouages ont été réalisés");
            exit(0);
        }

        sem_post(&sem_end_tattoo); // On a finit le tattoo

    } while (param->nombre_tatoos != nombre_tattoo_eff);

    return EXIT_SUCCESS;

}

int main(int argc, char *argv[]) {

    if (argc < 4){
        fprintf(stderr,"Not enought arguments");
        return EXIT_FAILURE;
    }

    int number_tattoos = atoi(argv[1]);
    int number_clients = atoi(argv[2]);
    int number_tatoueurs = atoi(argv[3]);
    int number_sieges_salle_attente = atoi(argv[4]);

    sem_init(&sem_porte,0,1);
    sem_init(&sem_seats,0,number_sieges_salle_attente);
    sem_init(&sem_fauteuils,0,number_tatoueurs);
    sem_init(&sem_start_tattoo,0,0);
    sem_init(&sem_end_tattoo,0,0);

    pthread_mutex_init(&promenadance,NULL);
    pthread_mutex_init(&tattoueur_reveil,NULL);
    pthread_mutex_init(&mut_tattoo_eff,NULL);

    printf(ANSI_COLOR_GREEN "Number of tattoos: %i" ANSI_COLOR_RESET "\n", number_tattoos);
    printf(ANSI_COLOR_GREEN "Number of clients: %i" ANSI_COLOR_RESET "\n", number_clients);
    printf(ANSI_COLOR_GREEN "Number of tattoueurs: %i" ANSI_COLOR_RESET "\n", number_tatoueurs);
    printf(ANSI_COLOR_GREEN "Number of sieges: %i" ANSI_COLOR_RESET "\n", number_sieges_salle_attente);

    assert(randomWalk(WALK_MIN_T,WALK_MAX_T) <= WALK_MAX_T && randomWalk(WALK_MIN_T,WALK_MAX_T) >= WALK_MIN_T);
    assert(randomTatoo(TATOO_MIN_T, TATOO_MAX_T) <= TATOO_MAX_T && randomTatoo(TATOO_MIN_T, TATOO_MAX_T) >= TATOO_MIN_T);

    pthread_t threads_clients[number_clients];
    pthread_t threads_tattoo[number_clients];

    param_t params;
    params.nombre_tatoos = number_tattoos;
    params.nombre_siege_disponible = number_sieges_salle_attente;

    for (int i = 0; i < number_clients; ++i)
    {
        params.id_thread = i;
        int code = pthread_create(&threads_clients[i], NULL, promenade, &params);
        assert(code == 0);

        int codeJoin = pthread_join(threads_clients[i], NULL) != 0;
        assert(codeJoin == 0);
    }

    for (int i = 0; i < number_tatoueurs; ++i)
    {
        int code = pthread_create(&threads_tattoo[i], NULL, tattoueur, &params);
        assert(code == 0);

        int codeJoin = pthread_join(threads_tattoo[i], NULL) != 0;
        assert(codeJoin == 0);
    }

    sem_destroy(&sem_porte);
    sem_destroy(&sem_seats);
    sem_destroy(&sem_fauteuils);
    sem_destroy(&sem_start_tattoo);
    sem_destroy(&sem_end_tattoo);

    pthread_mutex_destroy(&promenadance);
    pthread_mutex_destroy(&tattoueur_reveil);
    pthread_mutex_destroy(&mut_tattoo_eff);

    return 0;
}
