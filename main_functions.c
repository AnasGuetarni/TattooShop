/*
 * @author Group12 - Guetarni Anas - Gay Melvin - Keraim Marwan
 * @file : main_functions.c
 * @Contain all functions needed in the program
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>
#include <time.h>
#include <errno.h>
#include <unistd.h>
#include "main_functions.h"

/*
 * Fonction randomWalk
 * return a random walk time
 * using rand_r which is tread-safe
 */
int randomWalk(int value_min, int value_max){
	return rand_r(&seed)%(value_max-value_min)+value_min;
}

/*
 * Fonction randomTatoo
 * return a random time to make a tattoo
 * using rand_r which is tread-safe
 */
int randomTatoo(int value_min, int value_max){
	return rand_r(&seed)%(value_max-value_min)+value_min;
}

/*
 * Function stats
 * permit to show statistics
 */
void stats()
{
	int i = 0, j = 0;

	while(stats_client[i] != -999)
	{
		fprintf(stderr, "Le client : %d a été tatoué :%d fois \n", i,stats_client[i]);
		i++;
	}

	while(stats_tattooist[j] != -999)
	{
		fprintf(stderr, "Le tatoueur : %d à tattouer %d fois \n", j, stats_tattooist[j]);
		j++;
	}
}

/*
 * Fonction "*client"
 * this fonction contain the comportement of client threads
 *
 * initialization of *param at the type param_t_client with *params wich was passed in parameters
 * First time client will call the fonction randomWalk to go for a walk, so he goes into a state of sleep
 * Once the walk is over he calls the function "salle_attente" with param in parameter
 */
void *client(void *params){

	struct timespec sleep_time_client;

	param_t_client *param = (param_t_client*)params;

  #ifdef DEBUG
	 printf(ANSI_COLOR_MAGENTA"Le thread %d rentre en promenade\n"ANSI_COLOR_RESET, param->id_thread_client);
  #endif

	sleep_time_client.tv_sec = randomWalk(WALK_MIN_T, WALK_MAX_T);
  sleep_time_client.tv_nsec = randomWalk(WALK_MIN_T, WALK_MAX_T);

  #ifdef DEBUG
	 printf(ANSI_COLOR_GREEN ".tvsec promenade: %d\n" ANSI_COLOR_RESET, (int)sleep_time_client.tv_sec);
  #endif

	assert(nanosleep(&sleep_time_client,&sleep_time_client) == 0);

  #ifdef DEBUG
	 printf(ANSI_COLOR_MAGENTA "Le thread %d rentre de promenade\n" ANSI_COLOR_RESET, param->id_thread_client);
  #endif

	salle_attente(param);

	return NULL;
}


/*
 * Fonction "salle_attente"
 *
 * This function represent the waiting room of the tattoo shop
 * When a thread (customer) arrives in, the waiting room decreases by 1 the number of available seats
 *
 * If the number of available seats drops below 0 when a customer enters, the number of seats available is reset to 0
 * The customer leaves for a walk
 *
 * Else the thread will wait until a tattooist is available with using the semaphore
 * once tattooist is available we increment the number of available seats then we launch the tattoo and client increment the number of tattoos he has
 * once this tattoo was finished we increment the nb_tattouage and the customer calls the "client" function again (so go back on a walk)
 */
void salle_attente(param_t_client *params) {
	pthread_mutex_lock(&promenadance);
	nombre_siege_disponible--;

  #ifdef DEBUG
	 printf(ANSI_COLOR_RED "On enleve un siege disponible: %d avec le thread %d\n"ANSI_COLOR_RESET, nombre_siege_disponible, params->id_thread_client);
  #endif

  pthread_mutex_unlock(&promenadance);

	if (nombre_siege_disponible < 0)
	{
		pthread_mutex_lock(&promenadance);
		nombre_siege_disponible = 0;
		pthread_mutex_unlock(&promenadance);

	#ifdef DEBUG
		  printf("Aucun siège disponible donc le thread %d part en vadrouille\n", params->id_thread_client);
	#endif

	client(params);
	}


  #ifdef DEBUG
	 printf(ANSI_COLOR_YELLOW "devant salle attente %d \n" ANSI_COLOR_RESET, params->id_thread_client);
  #endif

  sem_wait(&sem_seats); // look if tattooist was available

    // waiting seat was free
	pthread_mutex_lock(&promenadance);
	nombre_siege_disponible++;

  #ifdef DEBUG
	 printf(ANSI_COLOR_RED "On ajoute un siege disponible: %d avec le thread %d\n"ANSI_COLOR_RESET, nombre_siege_disponible, params->id_thread_client);
  #endif

	pthread_mutex_unlock(&promenadance);

  #ifdef DEBUG
	 printf(ANSI_COLOR_BLUE "Le thread %d va se faire tatouer\n"ANSI_COLOR_RESET, params->id_thread_client);
  #endif

    // Thread strats to get tattooed
    sem_post(&sem_start_tattoo);  // launch tattoo
    sem_wait(&sem_end_tattoo); // wait until the end of the tattoo
    sem_post(&sem_seats); // tattoo seat free

	// Increment the number of tattoo he had
	pthread_mutex_lock(&stats_client_mut);
	stats_client[params->id_thread_client]++;
	pthread_mutex_unlock(&stats_client_mut);

	client(params); // Go back to walk
}

void *tatoueur(void *params)
{
	param_t_tattoo *param = (param_t_tattoo*) params;

	while (nombre_tattoo_eff != param->nombre_tatoos) {
		sem_wait(&sem_start_tattoo); // Attend un client
		tattouage(params);
	}
	return NULL;
}


/*
 * Fonction "tattouage"
 *
 * This function represent the action that the tattooist performs on the client
 * First time tattooist will call the fonction randomTatoo to make a tattoo, so he goes into a state of sleep
 *
 * after the number of tattoos performed and the number of tattoos performed by the tattooist are incremented
 *
 * If the total number of tattoos is different from the number of tattoos wanted then we remind the "tatoueur" function
 * Otherwise it means that we have done all our tattos so we finish the program
*/
void tattouage (param_t_tattoo *params){

	struct timespec sleep_time_tattoo;

  #ifdef DEBUG
	 printf("Le tattoueur n° %d va commencer \n", params->id_thread_tattoueurs);
  #endif

	sleep_time_tattoo.tv_sec = randomTatoo(TATOO_MIN_T, TATOO_MAX_T);
  sleep_time_tattoo.tv_nsec = randomTatoo(TATOO_MIN_T, TATOO_MAX_T);

  #ifdef DEBUG
	 printf(ANSI_COLOR_GREEN".tvsec tattoo: %d\n"ANSI_COLOR_RESET, (int)sleep_time_tattoo.tv_sec);
  #endif

	assert(nanosleep(&sleep_time_tattoo,&sleep_time_tattoo) == 0);

	pthread_mutex_lock(&mut_tattoo_eff);
	nombre_tattoo_eff++;
	printf(ANSI_COLOR_YELLOW "Nombre de tattoo effectifs: %d\n" ANSI_COLOR_RESET, nombre_tattoo_eff);
	pthread_mutex_unlock(&mut_tattoo_eff);

    // Incrementation of the number of tattoos performed
	pthread_mutex_lock(&stats_tattooist_mut);
	stats_tattooist[params->id_thread_tattoueurs]++;
	pthread_mutex_unlock(&stats_tattooist_mut);

	sem_post(&sem_end_tattoo); // tattoo finished

	if (params->nombre_tatoos != nombre_tattoo_eff)
	{
		tatoueur(params);
	}
	else
	{
		printf("Tous les tattouages ont été réalisés\n");
		exit(0);
	}
}
