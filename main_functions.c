#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>
#include <time.h>
#include "main_functions.h"

int randomWalk(int value_min, int value_max){
    return rand_r(&seed)%(value_max-value_min)+value_min;
}

int randomTatoo(int value_min, int value_max){
    return rand_r(&seed)%(value_max-value_min)+value_min;
}

void *client(void *params){

	struct timespec sleep_time_client;

	param_t_client *param = (param_t_client*)params;

	printf(ANSI_COLOR_MAGENTA"Le thread %d rentre en promenade\n"ANSI_COLOR_RESET, param->id_thread_client);

	sleep_time_client.tv_sec = randomWalk(WALK_MIN_T, WALK_MAX_T);
  sleep_time_client.tv_nsec = randomWalk(WALK_MIN_T, WALK_MAX_T);

	printf(ANSI_COLOR_GREEN ".tvsec promenade: %d\n" ANSI_COLOR_RESET, (int)sleep_time_client.tv_sec);

	assert(nanosleep(&sleep_time_client,&sleep_time_client) == 0);

	printf(ANSI_COLOR_MAGENTA "Le thread %d rentre de promenade\n" ANSI_COLOR_RESET, param->id_thread_client);

	salle_attente(param);

	return NULL;
}

void salle_attente(param_t_client *params) {
	pthread_mutex_lock(&promenadance);
	nombre_siege_disponible--;
	printf(ANSI_COLOR_RED "On enleve un siege disponible: %d avec le thread %d\n"ANSI_COLOR_RESET, nombre_siege_disponible, params->id_thread_client);
	pthread_mutex_unlock(&promenadance);

	if (nombre_siege_disponible < 0)
	{
		pthread_mutex_lock(&promenadance);
		nombre_siege_disponible = 0;
		pthread_mutex_unlock(&promenadance);
		printf("Aucun siège disponible donc le thread %d part en vadrouille\n", params->id_thread_client);
		client(params);
	}

	// Porte d'entrée : premier arrivé, premier servi
	printf(ANSI_COLOR_YELLOW "devant salle attente %d \n" ANSI_COLOR_RESET, params->id_thread_client);
	sem_wait(&sem_seats); // Regarde si un tatoueur est disponible

	// On libère le siège
	pthread_mutex_lock(&promenadance);
	nombre_siege_disponible++;
	printf(ANSI_COLOR_RED "On ajoute un siege disponible: %d avec le thread %d\n"ANSI_COLOR_RESET, nombre_siege_disponible, params->id_thread_client);
	pthread_mutex_unlock(&promenadance);

	printf(ANSI_COLOR_BLUE "Le thread %d va se faire tatouer\n"ANSI_COLOR_RESET, params->id_thread_client);
	// Le Thread peut commencer a se faire tatouer
	sem_post(&sem_start_tattoo);  // Lance un tatoueur
	sem_wait(&sem_end_tattoo); // attend la fin de son tattoo
	sem_post(&sem_seats);  // libère le fauteuil ou il s'est fait tattouer

	params->nb_tattouage++; // incrémente le nombre de tatouage fait sur son corps

	client(params); // Retourne se promener
}

void *tatoueur(void *params)
{
	param_t_tattoo *param = (param_t_tattoo*) params;

	while (nombre_tattoo_eff != param->nombre_tatoos) {
		printf("tattoo begin sem_start\n");
		sem_wait(&sem_start_tattoo); // Attend un client
		printf("tattoo end sem_start\n");
		tattouage(params);
	}
  printf("Tous les tattouages ont été réalisés\n");
	return NULL;
}

void tattouage (param_t_tattoo *params){

	struct timespec sleep_time_tattoo;
	int id = params->id_thread_tattoueurs;

	printf("Le tattoueur n° %d va commencer \n", id);

	sleep_time_tattoo.tv_sec = randomTatoo(TATOO_MIN_T, TATOO_MAX_T);
  sleep_time_tattoo.tv_nsec = randomTatoo(TATOO_MIN_T, TATOO_MAX_T);

	printf(ANSI_COLOR_GREEN".tvsec tattoo: %d\n"ANSI_COLOR_RESET, (int)sleep_time_tattoo.tv_sec);

	assert(nanosleep(&sleep_time_tattoo,&sleep_time_tattoo) == 0);

	pthread_mutex_lock(&mut_tattoo_eff);
	nombre_tattoo_eff++;
	printf(ANSI_COLOR_YELLOW "Nombre de tattoo effectifs: %d\n" ANSI_COLOR_RESET, nombre_tattoo_eff);
	pthread_mutex_unlock(&mut_tattoo_eff);

	// Incrementer nb de tattoo fait par le tatoueur
	params->nb_tattoo_eff++;
  params->nombre_tattoos_per_tattoo++;
  printf("Le nombre de tattouage pour le tattoueur numéro %d est de %d\n", id, params->nombre_tattoos_per_tattoo);
	sem_post(&sem_end_tattoo); // On a finit le tattoo

	if (params->nombre_tatoos != nombre_tattoo_eff)
	{
		tatoueur(params);
	}
	else
	{
		exit(0);
	}
}
