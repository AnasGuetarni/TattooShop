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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
int nombre_siege_disponible;
int nombre_tattoo_eff;
struct timespec sleep_time_tattoo;
struct timespec sleep_time;

void *client(void *id_thread){
    //int id = *((int*) id_thread);

    printf(ANSI_COLOR_MAGENTA"Le thread %i rentre en promenade\n"ANSI_COLOR_RESET, *((int*)id_thread));

    sleep_time.tv_sec = randomWalk(WALK_MIN_T, WALK_MAX_T);
    //sleep_time.tv_sec = 5;
    sleep_time.tv_nsec = 0;

    printf(ANSI_COLOR_GREEN".tvsec promenade: %i\n"ANSI_COLOR_RESET,(int) sleep_time.tv_sec);

    assert(nanosleep(&sleep_time,&sleep_time) == 0);

    printf(ANSI_COLOR_MAGENTA"Le thread %i rentre de promenade\n"ANSI_COLOR_RESET, *((int*)id_thread));

  	salle_attente((int*)id_thread);

    return EXIT_SUCCESS;
}

void salle_attente(int *id_thread) {

    if (nombre_siege_disponible <= 0)
    {
        printf("Aucun siège disponible donc le thread %i part en vadrouille\n", *id_thread);
        client((void*)id_thread);
    }

	pthread_mutex_lock(&promenadance);
    nombre_siege_disponible--;
    printf(ANSI_COLOR_RED "On enleve un siege disponible: %i avec le thread %i\n"ANSI_COLOR_RESET, nombre_siege_disponible, *id_thread);
	pthread_mutex_unlock(&promenadance);

	/* Porte d'entrée : premier arrivé, premier servi */
	//sem_wait(&porte);
    sem_wait(&sem_fauteuils); // Regarde si un tatoueur est disponible
    //sem_post(&porte);

	/* On libère le siège  */
	pthread_mutex_lock(&promenadance_end);
	nombre_siege_disponible++;
	printf(ANSI_COLOR_RED "On ajoute un siege disponible: %i avec le thread %i\n"ANSI_COLOR_RESET, nombre_siege_disponible, *id_thread);
	pthread_mutex_unlock(&promenadance_end);

    printf(ANSI_COLOR_BLUE "Le thread %i va se faire tatouer\n"ANSI_COLOR_RESET, *id_thread);
	/* Le Thread peut commencer a se faire tatouer */
    sem_post(&sem_start_tattoo);  // Lance un tatoueur
    sem_wait(&sem_end_tattoo); // attend la fin de son tattoo
    sem_post(&sem_fauteuils);  // libère le fauteuil ou il s'est fait tattouer

    client((void*)id_thread); // Retourne se promener
}
/*
void *tatoueur(void *params)
{
    param_t_tattoo *param = (param_t_tattoo*) params;

    while (nombre_tattoo_eff != param->nombre_tatoos) {
        printf("tattoo begin sem_start\n");
        sem_wait(&sem_start_tattoo); // Attend un client
        printf("tattoo end sem_start\n");
        tattouage(params);
    }
}

void tattouage (param_t_tattoo *params){
    int id = params->id_thread_tattoueurs;
        
	printf("Le tattoueur va commencer pour le thread tattoo %i\n", id);

	sleep_time_tattoo.tv_sec = randomTatoo(TATOO_MIN_T, TATOO_MAX_T);
	sleep_time_tattoo.tv_nsec = 0;

	printf(ANSI_COLOR_GREEN".tvsec tattoo: %i\n"ANSI_COLOR_RESET,(int) sleep_time_tattoo.tv_sec);

	assert(nanosleep(&sleep_time_tattoo,&sleep_time_tattoo) == 0);

	pthread_mutex_lock(&mut_tattoo_eff);
	nombre_tattoo_eff++;
	printf(ANSI_COLOR_YELLOW "Nombre de tattoo effectifs: %i\n" ANSI_COLOR_RESET, nombre_tattoo_eff);
	pthread_mutex_unlock(&mut_tattoo_eff);

	sem_post(&sem_end_tattoo); // On a finit le tattoo
	
	if (params->nombre_tatoos != nombre_tattoo_eff)
	{
        tatoueur(params);
	}
	else
	{
		printf(ANSI_COLOR_CYAN "Tous les tattouages ont été réalisés\n" ANSI_COLOR_RESET);
		exit(0);
	}
}
*/
int main(int argc, char *argv[]) {

    if (argc < 4){
        fprintf(stderr,"Not enought arguments");
        return EXIT_FAILURE;
    }

    int number_tattoos = atoi(argv[1]);
    int number_clients = atoi(argv[2]);
    int number_tatoueurs = atoi(argv[3]);
    int number_sieges_salle_attente = atoi(argv[4]);

    sem_init(&sem_fauteuils, 0, (unsigned int) number_tatoueurs);
    sem_init(&sem_start_tattoo,0,0);
    sem_init(&sem_end_tattoo,0,0);
    sem_init(&porte,0,1);

    pthread_mutex_init(&promenadance,NULL);
    pthread_mutex_init(&promenadance_end,NULL);
    pthread_mutex_init(&tattoueur_reveil,NULL);
    pthread_mutex_init(&mut_tattoo_eff,NULL);

    printf(ANSI_COLOR_GREEN "Number of tattoos: %i" ANSI_COLOR_RESET "\n", number_tattoos);
    printf(ANSI_COLOR_GREEN "Number of clients: %i" ANSI_COLOR_RESET "\n", number_clients);
    printf(ANSI_COLOR_GREEN "Number of tattoueurs: %i" ANSI_COLOR_RESET "\n", number_tatoueurs);
    printf(ANSI_COLOR_GREEN "Number of sieges: %i" ANSI_COLOR_RESET "\n", number_sieges_salle_attente);

    assert(randomWalk(WALK_MIN_T,WALK_MAX_T) <= WALK_MAX_T && randomWalk(WALK_MIN_T,WALK_MAX_T) >= WALK_MIN_T);
    assert(randomTatoo(TATOO_MIN_T, TATOO_MAX_T) <= TATOO_MAX_T && randomTatoo(TATOO_MIN_T, TATOO_MAX_T) >= TATOO_MIN_T);

    pthread_t threads_clients[number_clients];
    pthread_t threads_tattoo[number_tatoueurs];

    int id_client[number_clients];
    param_t_tattoo params_tattoueurs[number_tatoueurs];

	nombre_tattoo_eff = 0;
    nombre_siege_disponible = number_sieges_salle_attente;
	   
	for (int i = 0; i < number_clients; i++)
	{
        id_client[i]=i;
		int code = pthread_create(&threads_clients[i], NULL, client, &id_client[i]);
		assert(code == 0);
	}
/*
    for (int i = 0; i < number_tatoueurs; i++)
    {
        params_tattoueurs[i].id_thread_tattoueurs = i;
        params_tattoueurs[i].nombre_tatoos = number_tattoos;
		int code = pthread_create(&threads_tattoo[i], NULL, tatoueur, &params_tattoueurs[i]);
		assert(code == 0);
    }
*/
    for (int k = 0; k < number_clients; k++){
      int codeJoin = pthread_join(threads_clients[k], NULL) != 0;
      assert(codeJoin == 0);
    }
/*
    for (int l = 0; l < number_tatoueurs; l++){
      int codeJoin = pthread_join(threads_tattoo[l], NULL) != 0;
      assert(codeJoin == 0);
    }
*/

    sem_destroy(&sem_fauteuils);
    sem_destroy(&sem_start_tattoo);
    sem_destroy(&sem_end_tattoo);
    sem_destroy(&porte);

    pthread_mutex_destroy(&promenadance);
    pthread_mutex_destroy(&promenadance_end);
    pthread_mutex_destroy(&tattoueur_reveil);
    pthread_mutex_destroy(&mut_tattoo_eff);

    return EXIT_SUCCESS;
}

#pragma clang diagnostic pop